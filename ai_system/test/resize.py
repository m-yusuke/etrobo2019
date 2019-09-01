from PIL import Image

if __name__ == '__main__':
    
    img_resize = Image.open('./snapshot.jpg')
    img_resize = img_resize.resize((2560, 1440))
    imgCrop = img_resize.crop((0, 720, 750, 1420)) # ((0, 390, 390, 719))
    imgCrop.save('./resize2.jpg')