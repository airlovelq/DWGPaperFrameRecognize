#!/usr/bin/env python3
#coding=utf-8
import numpy as np
import cv2 as cv
import os.path
def resizePic(filename):
    img = cv.imread(filename)
    imgdst = cv.resize(img,(128,128))

    #cv.namedWindow("Image")
    #cv.imshow("Image", imgdst)
    #cv.waitKey(0)
    #cv.destroyAllWindows()
    return imgdst

def getPicData(dir):
    imglist = []
    typelist = []

    if os.path.exists(dir):
        dirs = os.listdir(dir)
        for dirc in dirs:
            if os.path.isfile(dir+'/'+dirc):
                img = resizePic(dir+'/'+dirc)
                imglist.append(img)
                type = [0, 0]
                if dirc[0] == '0':
                    type[0] = 1
                else:
                    type[1] = 1
                typelist.append(type)
    else:
        print("dir not exists")
    return imglist,typelist

if __name__ == '__main__':
    str = "F:/AI/1.jpg"
    #resizePic(str)
    data,typel = getPicData('f:/AITrain')
    print(data)
    print(typel)