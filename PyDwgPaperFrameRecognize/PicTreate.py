#!/usr/bin/env python3
#coding=utf-8
import numpy as np
import cv2 as cv
import os.path

#图片前处理，灰度，二值化，提取轮廓，取最大外包分割，提取ROI
def treatePic(filename):
    img = cv.imread(filename)
    imgdst = cv.cvtColor(img,cv.COLOR_BGR2GRAY)#resize(img,(128,128))
    cv.bitwise_not(imgdst,imgdst)
    _,imgdst = cv.threshold(imgdst,20,255,cv.THRESH_BINARY)
    _, contours, hierarchy = cv.findContours(imgdst,cv.RETR_EXTERNAL,cv.CHAIN_APPROX_SIMPLE)
    rectmax = []
    for i in range(len(contours)):
        rect = cv.boundingRect(contours[i])
        if ( i == 0 ):
            rectmax.append(rect[0])
            rectmax.append(rect[1])
            rectmax.append(rect[0] + rect[2])
            rectmax.append(rect[1] + rect[3])
        rectmax = [min(rect[0],rectmax[0]),min(rect[1],rectmax[1]),max(rect[0]+rect[2],rectmax[2]),max(rect[1]+rect[3],rectmax[3])]
        #print (rectmax)
        #imgdst = cv.rectangle(imgdst, (0, 0), (50, 50), (255, 255, 0), 4, 1)
    #imgdst = cv.rectangle(imgdst,(rectmax[0],rectmax[1]),(rectmax[2],rectmax[3]),(255,255,255),4,1)
    imgdst = imgdst[rectmax[1]:rectmax[3],rectmax[0]:rectmax[2]]
    imgdst = cv.resize(imgdst, (128, 128))
    #cv.namedWindow("Image")
    #cv.imshow("Image", imgdst)
    #cv.waitKey(0)
    #cv.destroyAllWindows()
    return imgdst

#读取文件夹下的样本，返回图片数据和类型list
def getPicData(dir):
    imglist = []
    typelist = []

    if os.path.exists(dir):
        dirs = os.listdir(dir)
        for dirc in dirs:
            if os.path.isfile(dir+'/'+dirc):
                img = treatePic(dir+'/'+dirc)
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
    str = "F:/AITrain/0-yysjx-1.jpg"
    treatePic(str)
    #resizePic(str)
    #data,typel = getPicData('f:/AITrain')
    #print(data)
    #print(typel)
