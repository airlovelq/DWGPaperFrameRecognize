import tensorflow as tf
import PicTreate as PT
import numpy as np

def judge(picFolder,modelPath,checkpoint):
    datatest, typetest = PT.getPicData(picFolder)
    datatest = np.array(datatest)
    datatest = datatest.astype('float32')/255
    typetest = np.array(typetest)

    ret1 = []
    with tf.Session() as sess:
        saver = tf.train.import_meta_graph(modelPath)
        saver.restore(sess,tf.train.latest_checkpoint(checkpoint))
        graph = tf.get_default_graph()
        x = graph.get_tensor_by_name("x:0")
        y_ = graph.get_tensor_by_name("y_:0")
        keep_prob = graph.get_tensor_by_name("keep_prob:0")
        result = graph.get_tensor_by_name("result:0")
        acc = graph.get_tensor_by_name("acc:0")
        feed_dict1 = {x:datatest,y_:typetest,keep_prob:0.5}
        ret1 = sess.run(result, feed_dict1)
        print(ret1)
        print(sess.run(acc,feed_dict1))

    return ret1

if __name__ == '__main__':
    judge('f:/AITest1','f:/tensorsave/model.meta','f:/tensorsave/')

