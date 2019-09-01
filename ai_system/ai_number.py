import cv2
import numpy as np
import matplotlib.pyplot as plt
import keras
from PIL import Image
from keras.models import load_model
from keras.preprocessing.image import array_to_img, img_to_array,load_img

# https://note.nkmk.me/python-opencv-camera-to-still-image/

# 画像の切り取り
def resize(img_path,save_path):
    img = Image.open(img_path)
    img = img.resize((2560, 1440))
    resize_img = img.crop((0, 700, 750, 1400))# (0, 390, 390, 719)) # img.crop((0, 600, 750, 1300)) 3K画像の場合
    resize_img.save(save_path)


# 画像の色変換
def changeColor(img_path,save_path):
    img = cv2.imread(img_path)
    bgrLower = np.array([46,89,62])    # 抽出する色の下限(BGR)
    bgrUpper = np.array([255,255,255])    # 抽出する色の上限(BGR)
    img_mask = cv2.inRange(img, bgrLower, bgrUpper) # BGRからマスクを作成
    result = cv2.bitwise_and(img, img, mask=img_mask) # 元画像とマスクを合成
    saveImg = Image.fromarray(result.astype(np.uint8))
    saveImg.save(save_path)

# 学習モデルとの比較
def judge_number(picture):
    # モデルの読み込み
    model = load_model('./Model/etrobo.h5')
    X = []
    img = img_to_array(load_img(picture, target_size=(150, 150),color_mode='rgb'))
    X.append(img)

    X = np.asarray(X)
    X = X.astype('float32') / 255

    # モデルとの比較
    features = model.predict(X)
    # 結果の表示
    return features.argmax()+1

# メイン処理
def ai_main(img_path,save_path):
    resize(img_path,save_path)
    changeColor(save_path,save_path)
    res = judge_number(save_path)
    print(res)