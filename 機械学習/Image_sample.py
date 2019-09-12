import os
from Image_test import image_test
import keras
from keras.utils import np_utils
from keras.models import load_model

def main():
    #print(os.sys.path)
    model = load_model('./MODEL/sample1_model.h5')
    print("test")
    test(model)

def test(model):
    dire = ["1","2","3","4","5","6","7","8"]
    for i in dire:
        ima_te = image_test()
        image = ima_te.main(i)
        features = model.predict(image)
        print(features.argmax()+1)

if __name__ == "__main__":
    main()

