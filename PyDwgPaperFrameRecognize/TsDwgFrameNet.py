import tensorflow as tf
import numpy as np
import PicTreate as PT
import cv2 as cv
IMAGE_SIZE = 128
NUM_CHANNELS = 3
CONV1_DEEP = 32
CONV1_SIZE = 3
CONV2_SIZE = 3
CONV2_DEEP = 64
#INPUT_NODE = 640*640*3
OUTPUT_NODE = 2
LAYER1_NODE = 256
LEARNING_RATE = 0.0001
BATCH_SIZE = 5
DATA_SIZE = 299



def train(datatrain, typetrain, datatest, typetest, modelPath):
    x = tf.placeholder(tf.float32,[None,IMAGE_SIZE,IMAGE_SIZE,NUM_CHANNELS],name='x')
    y_ = tf.placeholder(tf.float32, [None, 2],name='y_')
    keep_prob_5 = tf.placeholder(tf.float32,name='keep_prob')
    keep_prob_75 = tf.placeholder(tf.float32)

    with tf.variable_scope('conv1'):
        weight1 = tf.get_variable('weight',[CONV1_SIZE,CONV1_SIZE,NUM_CHANNELS,CONV1_DEEP],initializer=tf.truncated_normal_initializer(stddev=0.1))
        bias1 = tf.get_variable('bias',[CONV1_DEEP],initializer=tf.constant_initializer(0.1))
        conv1 = tf.nn.conv2d(x, weight1, strides=[1, 1, 1, 1], padding='SAME')
        relu1 = tf.nn.relu(conv1 + bias1)

    with tf.variable_scope('pool1'):
        pool1 = tf.nn.max_pool(relu1, ksize=[1, 2, 2, 1], strides=[1, 2, 2, 1], padding='SAME')
    pool1 = tf.nn.dropout(pool1,keep_prob_5)

    with tf.variable_scope('conv2'):
        weight2 = tf.get_variable('weight',[CONV2_SIZE,CONV2_SIZE,CONV1_DEEP,CONV2_DEEP],initializer=tf.truncated_normal_initializer(stddev=0.1))
        bias2 = tf.get_variable('bias', [CONV2_DEEP], initializer=tf.constant_initializer(0.1))
        conv2 = tf.nn.conv2d(pool1, weight2,strides=[1,1,1,1],padding='SAME')
        relu2 = tf.nn.relu(conv2 + bias2)

    with tf.variable_scope('pool2'):
        pool2 = tf.nn.max_pool(relu2, ksize=[1, 2, 2, 1], strides=[1, 2, 2, 1], padding='SAME')
    pool2 = tf.nn.dropout(pool2,keep_prob_5)
    pool_shape = pool2.get_shape().as_list()

    nodes = pool_shape[1]*pool_shape[2]*pool_shape[3]
    print(pool_shape[0])
    print(pool_shape[1])
    print(pool_shape[2])
    print(pool_shape[3])
    print(nodes)
    reshaped = tf.reshape(pool2,[-1,nodes])
    with tf.variable_scope('fc1'):
        weightfc1 = tf.get_variable('weight',[nodes,LAYER1_NODE],initializer=tf.truncated_normal_initializer(stddev=0.1))
        biasfc1 = tf.get_variable('bias', [LAYER1_NODE] ,initializer=tf.constant_initializer(0.1) )
        fc1 = tf.nn.relu(tf.matmul(reshaped,weightfc1) + biasfc1)

    with tf.variable_scope('fc2'):
        weightfc2 = tf.get_variable('weight',[LAYER1_NODE,OUTPUT_NODE],initializer=tf.truncated_normal_initializer(stddev=0.1))
        biasfc2 = tf.get_variable('bias', [OUTPUT_NODE] ,initializer=tf.constant_initializer(0.1) )
        fc2 = tf.matmul(fc1,weightfc2) + biasfc2

    cross_entropy = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits(logits=fc2, labels=y_))
    train_step = tf.train.AdamOptimizer(0.001).minimize(cross_entropy)
    # 比较标签是否相等，再求的所有数的平均值，tf.cast(强制转换类型)
    accuracy = tf.reduce_mean(tf.cast(tf.equal(tf.argmax(fc2, 1), tf.argmax(y_, 1)), tf.float32),name="acc")
    result = tf.argmax(fc2, 1, name='result')
    with tf.Session() as sess:
        sess.run(tf.global_variables_initializer())
        for n in range(1):
            # 每次取50张图片
            for i in range(len(datatrain)//BATCH_SIZE):
                batch_x = datatrain[i * BATCH_SIZE: (i + 1) * BATCH_SIZE]
                batch_y = typetrain[i * BATCH_SIZE: (i + 1) * BATCH_SIZE]
                # 开始训练数据，同时训练三个变量，返回三个数据
                _, loss,pp1 = sess.run([train_step, cross_entropy,fc2], feed_dict={x: batch_x, y_: batch_y, keep_prob_5:0.5})
                #if (n * 16 + i) % 100 == 0:
                    # 获取测试数据的准确率
                acc = accuracy.eval({x: datatest, y_: typetest, keep_prob_5:0.5})
                print(tf.argmax(fc2, 1).eval({x: datatest, y_: typetest, keep_prob_5:0.5}))
                print('经过%d次训练，测试正确率为%f'%(n * len(datatrain)//50 + i, acc))
                #print(pp1.shape)
                #singletest = sess.run(tf.argmax(fc2, 1),feed_dict={x: imgsingle, keep_prob_5:0.5})
                #print(singletest)
        saver = tf.train.Saver()
        saver.save(sess, modelPath)#'f:/tensorsave/model')
