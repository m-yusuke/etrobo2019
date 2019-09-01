import cv2
import os
from PIL import Image

def resize(img_path):
    img = Image.open(img_path)
    imgCrop = img.crop((0, 600, 750, 1300))
    imgCrop.save(img_path)

def save_frame_camera_key():
    url = "http://raspberry.pi/?action=stream"
    cap = cv2.VideoCapture(url)

    if not cap.isOpened():
        return
        
    while True:
        ret, frame = cap.read()
        cv2.imshow("frame", frame)
        key = cv2.waitKey(1) & 0xFF
        if key == ord('c'):
            cv2.imwrite("cap.jpg", frame)
            break

    cv2.destroyWindow("frame")

if __name__ == "__main__":
    save_frame_camera_key()
    resize("./cap.jpg")