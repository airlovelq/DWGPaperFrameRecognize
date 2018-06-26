#!/usr/bin/env python3
#coding=utf-8
import sys
from PyQt5.QtCore import *
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
import DLgMain
import TsDwgFrameNet
import PicTreate as PT
import numpy as np

if __name__ == "__main__":
    app = QApplication(sys.argv)
    dlg = DLgMain.QDlgDwgFrameNet()
    dlg.show()
    app.exec()
    datatrain, typetrain = PT.getPicData(dlg.strDataPath)
    datatest, typetest = PT.getPicData(dlg.strTestPath)
    datatrain = np.array(datatrain)
    datatest = np.array(datatest)
    datatrain = datatrain.astype('float32') / 255
    datatest = datatest.astype('float32') / 255
    typetest = np.array(typetest)
    typetrain = np.array(typetrain)
    TsDwgFrameNet.train(datatrain, typetrain, datatest, typetest, dlg.strModelPath+'/'+dlg.strModelName)
