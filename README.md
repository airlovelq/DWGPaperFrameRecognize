# DWGPaperFrameRecognize
实现使用TensorFlow，识别与判断dwg文件中的图框
1、PaperFrameRecognize文件夹中为AutoCAD2013环境下的dwg文件分析及图框标定程序，最终导出图框与非图框的图片，作为训练样本。
2、PyDwgPaperFrameRecognize文件夹中为python文件，TensorFlow训练网络，样本选择对话框，图片预处理等
   DLGMain   主界面对话框，用于选择样本路径，模型保存路径等
   PicTreate   图片读取与前处理
   TrainProcess   模型训练主进程
   TsDwgFrameNet  神经网络定义
   TsJudge   神经网络保存模型的调用
