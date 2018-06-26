#!/usr/bin/env python3
#coding=utf-8

from PyQt5.QtCore import *
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
import sys
class QDlgDwgFrameNet(QDialog):
    def __init__(self,parent=None):
        super(QDlgDwgFrameNet,self).__init__(parent)
        self.setWindowTitle('图框cnn训练')

        labelX = QLabel()
        labelX.setText('训练样本路径:')
        btnX = QPushButton()
        btnX.setText('选择路径')
        btnX.clicked.connect(self.selDataPath)
        #btnX.setFixedWidth(150)
        self.ledtX = QLineEdit()
        self.ledtX.setText('')
        self.ledtX.setEnabled(False)
        layoutX = QHBoxLayout()
        layoutX.addWidget(labelX)
        layoutX.addWidget(self.ledtX)
        layoutX.addWidget(btnX)

        labelT = QLabel()
        labelT.setText('测试样本路径:')
        btnT = QPushButton()
        btnT.setText('选择路径')
        btnT.clicked.connect(self.selTestPath)
        # btnX.setFixedWidth(150)
        self.ledtT = QLineEdit()
        self.ledtT.setText('')
        self.ledtT.setEnabled(False)
        layoutT = QHBoxLayout()
        layoutT.addWidget(labelT)
        layoutT.addWidget(self.ledtT)
        layoutT.addWidget(btnT)

        labelS = QLabel()
        labelS.setText('模型保存路径:')
        btnS = QPushButton()
        btnS.setText('选择路径')
        btnS.clicked.connect(self.selModelPath)
        #btnS.setFixedWidth(150)
        self.ledtS = QLineEdit()
        self.ledtS.setText('')
        self.ledtS.setEnabled(False)
        layoutS = QHBoxLayout()
        layoutS.addWidget(labelS)
        layoutS.addWidget(self.ledtS)
        layoutS.addWidget(btnS)

        labelM = QLabel()
        labelM.setText('模型名称:')
        self.ledtM = QLineEdit()
        layoutM = QHBoxLayout()
        layoutM.addWidget(labelM)
        layoutM.addWidget(self.ledtM)


        sc = QSpacerItem(80, 40, QSizePolicy.Expanding, QSizePolicy.Minimum)
        btnOk = QPushButton()
        btnOk.setText('确定')
        btnOk.setFixedWidth(80)
        btnOk.clicked.connect(self.onOk)
        btnCancel = QPushButton()
        btnCancel.setText('取消')
        btnCancel.setFixedWidth(80)
        btnCancel.clicked.connect(self.close)
        layoutBtn = QHBoxLayout()
        layoutBtn.addSpacerItem(sc)
        layoutBtn.addWidget(btnOk)
        layoutBtn.addWidget(btnCancel)


        layout = QGridLayout()
        #layout.addWidget(label,0,0)
        #layout.addWidget(ledt,0,1)
        #layout.addWidget(btn,0,2)
        #layout.addWidget(btnOk,1,1)
        #layout.addWidget(btnCancel, 1, 2)
        layout.addLayout(layoutX, 0, 0)
        layout.addLayout(layoutT, 1, 0)
        layout.addLayout(layoutS, 2, 0)
        layout.addLayout(layoutM, 3, 0)
        layout.addLayout(layoutBtn, 4, 0)
        self.setLayout(layout)


        self.strDataPath = ''
        self.strModelPath = ''
        self.strTestPath = ''
        self.strModelName = ''

    def selDataPath(self):
        directory = QFileDialog.getExistingDirectory(self,"选取文件夹","C:/")  # 起始路径
        self.ledtX.setText(directory)

    def selTestPath(self):
        directory = QFileDialog.getExistingDirectory(self,"选取文件夹","C:/")  # 起始路径
        self.ledtT.setText(directory)

    def selModelPath(self):
        directory = QFileDialog.getExistingDirectory(self, "选取文件夹", "C:/")  # 起始路径
        self.ledtS.setText(directory)

    def onOk(self):
        self.strDataPath = self.ledtX.text()
        self.strModelPath = self.ledtS.text()
        self.strTestPath = self.ledtT.text()
        self.strModelName = self.ledtM.text()
        if (len(self.strModelPath)<=0) or (len(self.strModelPath)<=0) or (len(self.strTestPath)<=0) or (len(self.strModelName)<=0):
            QMessageBox.information(self,  # 使用infomation信息框
                                    "提示",
                                    "路径不正确，请重新指定！",
                                    QMessageBox.Ok)
            return
        self.accept()


if __name__ == "__main__":
    app = QApplication(sys.argv)
    dlg = QDlgDwgFrameNet()
    dlg.show()
    app.exec()

