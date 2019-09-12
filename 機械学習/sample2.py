import os
import keras
from keras.utils import np_utils
from keras.models import Sequential
from keras.layers.convolutional import Conv2D, MaxPooling2D
from keras.layers.core import Dense, Dropout, Activation, Flatten
from Image1 import Image_file
from Image_test import image_test


def main():
    image_file = Image_file()
    X_train, X_test, y_train, y_test = image_file.file_insert()
    model = Model(X_train)
    print("study start")
    #訓練
    model.fit(X_train, y_train, epochs=10, verbose=1)

    model.save("./MODEL/sample1_model.h5")
    #評価 & 評価結果出力
    print(model.evaluate(X_test, y_test))

    print("test")
    #test(model)

def Model(X_train):
    model = Sequential()

    model.add(Conv2D(32, (3, 3), padding='same',input_shape=X_train.shape[1:]))
    model.add(Activation('relu'))
    model.add(Conv2D(32, (3, 3)))
    model.add(Activation('relu'))
    model.add(MaxPooling2D(pool_size=(2, 2)))
    model.add(Dropout(0.25))
    
    model.add(Conv2D(64, (3, 3), padding='same'))
    model.add(Activation('relu'))
    model.add(Conv2D(64, (3, 3)))
    model.add(Activation('relu'))
    model.add(MaxPooling2D(pool_size=(2, 2)))
    model.add(Dropout(0.25))
    
    model.add(Flatten())
    model.add(Dense(512))
    model.add(Activation('relu'))
    model.add(Dropout(0.5))
    model.add(Dense(8))
    model.add(Activation('softmax'))
    
    # コンパイル
    model.compile(loss='categorical_crossentropy',optimizer='SGD',metrics=['accuracy'])    
    return model

def test(model):
    dire = ["1","2","3","4","5","6","7","8"]
    for i in dire:
        ima_te = image_test()
        image = ima_te.main(i)
        features = model.predict(image)
        print(features.argmax()+1)

if __name__ == "__main__":
    main()

