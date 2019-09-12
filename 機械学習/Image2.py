import os
import numpy as np
from PIL import Image
import cv2
from keras.utils import np_utils
import numpy as np
import matplotlib.pyplot as plt
from keras.models import load_model
from sklearn.model_selection import train_test_split
from keras.preprocessing.image import array_to_img, img_to_array,load_img


class Image():
    def __init__(self):
        self.urls = "./resize/"
        self.directory = ["1","2","3","4","5","6","7","8"]
        self.list_x = []
        self.list_y = []
        #self.lists=[]

    def list_insert(self,path,num):
        count=0
        for file in os.listdir(path):
            if count == 100:
                break
            if not file == ".DS_Store":
                temp_img = load_img(path+"/"+file, target_size=(64,64))
                print(temp_img)
                count+=1

    def data_load(self):
        for dire in self.directory:
            self.list_insert(self.urls+dire,dire)

    def list_append(self):
        pass
            

if __name__ == "__main__":
    image = Image()
    image.data_load()
