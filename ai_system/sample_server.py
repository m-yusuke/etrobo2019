#!/usr/bin/env python
# coding: utf-8
from functools import reduce
import struct
import numpy
import cv2
import datetime

from ai_number import ai_main
from sample_types import CommandCode
from sample_types import ResponseCode
from sample_types import Color
from SerialInterface import SerialInterface

port = "/dev/tty.MindstormsEV3-SerialPor" # 環境に合わせて変更して下さい
baud = 115200
picture = "picture/snapshot.jpg"

# 座標IDとピクセル座標の連想リスト(ピクセル座標はカメラ位置に応じて調整して下さい)
dots = {   1 : (257, 253),  2 : (446, 186),  3 : (583, 140),  4 : (689, 105),   5 : (445, 330),  6 : (625, 243),  7 : (750, 183),  8 : (842, 139),   9 : (710, 445), 10 : (860, 320), 11 : (953, 237), 12 : (1023, 180),  13 : (1075, 598), 14 : (1155, 415), 15 : (1200, 305), 16 : (1240, 232) }
# dots = {   1 : (440, 253),  2 : (596, 186),  3 : (705, 140),  4 : (657, 330),   5 : (889, 182),  6 : (927, 425),  7 : (1027, 315),  8 : (1101, 235) }

##############################
# 要求パケットを解読する関数     #
##############################
def decodePacket(command, parameter):
    # 引数チェック
    if command != CommandCode.All and command != CommandCode.Specific: # 未定義のコマンドコード
        errmsg = "unsupported command"
        return None, errmsg

    points   = [] # 色判定対象ピクセル座標のリスト
    points = dots.values()
    #if command == CommandCode.All: # 全座標色取得コマンド
    #    points = dots.values() # 全サークル分の座標リストを作る
    #elif command == CommandCode.Specific: # 指定座標色取得コマンド
    #    num_of_dots = int(parameter[0]) # 座標数
    #    for i in range(num_of_dots):
    #        # ネットワークバイトオーダー(!)で座標IDを読み出す
    #        dot_id = struct.unpack("!H", parameter[2*i+1:2*i+3])[0]
            # 座標IDに対応するピクセル座標をリストへ追加
    #        points.append( dots[dot_id] )
    return points, None

##############################
# 応答パケットを生成する関数     #
##############################
def encodePacket(rescode, result, errmsg=None):
    # 引数チェック：問題がある場合はエラー応答用のメッセージを設定
    if rescode != ResponseCode.Color and rescode != ResponseCode.Error: # 未定義のレスポンスコード
        lrescode = ResponseCode.Error
        lerrmsg = "server internal error"
    elif rescode == ResponseCode.Color and (len(result) > 84): # 応答座標数がプロトコルの規定上限を超えている
        lrescode = ResponseCode.Error
        lerrmsg = "response size is too big"
    else: # 引数が正常のケース
        lrescode = rescode
        lerrmsg = errmsg
    
    if lrescode == ResponseCode.Color: # 座標色応答のケース
        # ネットワークバイトオーダー(!)で座標ID,色コードをバイナリ化
        data = struct.pack("B", len(result)) # 座標数
        for item in result:
            data += struct.pack(
                "!HB", 
                [k for k, (x, y) in dots.items() if (x == item[0] and y == item[1])][0], # ピクセル座標から座標IDへの変換
                item[2] # ピクセル座標の色コード
            )
    else: # エラー応答のケース
        # エラーメッセージをUTF8エンコードでバイナリ化
        data = struct.pack(str(len(lerrmsg)) + "s", bytes(lerrmsg, 'utf-8'))

    return lrescode, struct.pack("B"*len(data), *data)

#####################################################
# カメラ画像上の指定座標のブロック色を判定する関数          #
# [(x座標,y座標,色),..]を返す                          #
#####################################################
def detectColor(points):
    # ローカルカメラのデバイスID
    DEVICE_ID = 0

    # Raspberry Pi3（カメラシステム）のURL
    url = "http://192.168.11.100/?action=stream"

    # VideoCaptureのインスタンスを作成する。
    cap = cv2.VideoCapture(url) # カメラシステムを使う場合
    # cap = cv2.VideoCapture(DEVICE_ID) # このpythonスクリプトを実行するPCのカメラを使う場合コメントアウトして下さい

    # カメラFPSを30FPSに設定
    cap.set(cv2.CAP_PROP_FPS, 30)

    # カメラ画像の横幅を1280に設定
    cap.set(cv2.CAP_PROP_FRAME_WIDTH, 1280)

    # カメラ画像の縦幅を720に設定
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 720)

    # ピクセル配列をゼロ初期化
    img = numpy.zeros((720, 1280, 3), dtype=numpy.uint8)
    
    result = [] # 判定結果のリスト
    if cap.isOpened():
        # 画像をキャプチャ
        ret, img = cap.read()

        for (x, y) in points:
            # RGBからHSVへ変換
            pixelValue = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)[y, x]
            
            # 色判定
            col = Color.getColor(pixelValue) # HSVから色コードへ変換
            result.append((x, y, col)) # (x座標,y座標,色)タプルをリストへ追加

            # 色情報を画像へ付加
            dot_num = [k for k, v in dots.items() if (x == v[0] and y == v[1])][0]
            H = str(pixelValue[0])
            S = str(pixelValue[1])
            V = str(pixelValue[2])
            cv2.circle(img,(x, y), 3, (255,255,255), -1)
            cv2.putText(img, ("[%d]:" % dot_num) + Color.toColorName(col), (x+10, y+5), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255,255,255), 1, cv2.LINE_AA)
            cv2.putText(img, H+","+S+","+V, (x+10, y+25), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255,255,255), 1, cv2.LINE_AA)

        # 画像をJPEGファイルへ保存(座標の調整用途)
        cv2.imwrite(picture,img)
        # cv2.imwrite('snapshot_{0:%Y%m%d_%H%M%S}.jpg'.format(datetime.datetime.now()), img)

    # キャプチャ終了
    cap.release()
    return result

################################
#        AI での数値判定         #
################################
def callAI():
    ai_main(picture,"picture/snapshot2.jpg")


##############
# メイン関数 #
##############
if __name__ == '__main__':
    # bluetooth通信用COMポートを開通する
    si = SerialInterface()
    si.open(port, baud, None)
 
    try:
        # クライアントから要求パケットを受信する（受信するまでブロックすることに注意）
        command, parameter = si.read()
        print("(command, parameter) = ", (command, parameter))

        # parameterを解読し色判定対象のピクセル座標のリストを作る
        points, errmsg = decodePacket(command, parameter)

        # 色判定結果から応答パケットを作る
        if points is not None:
            # 座標色応答
            response, parameter = encodePacket(ResponseCode.Color, detectColor(points))
        else:
            # エラー応答：座標IDからピクセル座標への特定ができなかったケース
            response, parameter = encodePacket(ResponseCode.Error, None, errmsg)
        # print("(response, parameter) = ", (response, parameter)) # ちょっとこいつの意味がわから無い。

        # 応答パケットを送信する
        si.write(response, parameter)
        si.close()
        callAI()

    except KeyboardInterrupt: # キーボード割り込み(Ctrl + C)
        # COMポートを閉じる
        si.close()
