from flask import Flask
from flask import render_template
from flask import request
from serial import Serial
import time

arduino = Serial('/dev/ttyAMA0', 9600, timeout=1)
app = Flask(__name__)
# 命令执行顺序控制变量，只有大于当前值的命令可以执行
lastseq = 0
# 资源锁，不等于0时表示上锁，无法被其他线程获取，直到释放
lock = 0
@app.route('/')
def index():
    return render_template('index.html', seq=lastseq) 


@app.route('/forward')
def forward():
    global lastseq
    left = request.args.get('left', '255')
    right = request.args.get('right', '255')
    seq = int(request.args.get('seq'))
    # 获取往串口写数据的资源权限
    getLock()
    try:
        # 控制命令执行顺序，避免异步操作顺序异常
        if seq <= lastseq:
            return 'seq error'
        lastseq = seq
        arduino.write(b'f,')
        arduino.write(bytes(left +"," + right, 'ascii'))
        arduino.write(b';')
        return 'forward, %s, %s' % (left, right)
    finally:
        # 释放串口资源
        releaseLock()

@app.route('/brake')
def brake():
    global lastseq
    seq = int(request.args.get('seq'))
    # 获取往串口写数据的资源权限
    getLock()
    try:
        # 控制命令执行顺序，避免异步操作顺序异常
        if seq <= lastseq:
            return 'seq error'
        lastseq = seq

        arduino.write(b's;')
        return 'brake'
    finally:
        # 释放串口资源
        releaseLock()

@app.route('/backward')
def backward():
    global lastseq
    left = request.args.get('left', '255')
    right = request.args.get('right', '255')
    seq = int(request.args.get('seq'))
    # 获取往串口写数据的资源权限
    getLock()
    try:
        # 控制命令执行顺序，避免异步操作顺序异常
        if seq <= lastseq:
            return 'seq error'
        lastseq = seq
        arduino.write(b'b,')
        arduino.write(bytes(left +"," + right, 'ascii'))
        arduino.write(b';')
        return 'backward, %s, %s' % (left, right)
    finally:
        # 释放串口资源
        releaseLock()

@app.route('/right')
def turnRight():
    global lastseq
    left = request.args.get('left', '255')
    right = request.args.get('right', '255')
    seq = int(request.args.get('seq'))
    # 获取往串口写数据的资源权限
    getLock()
    try:
        # 控制命令执行顺序，避免异步操作顺序异常
        if seq <= lastseq:
            return 'seq error'
        lastseq = seq
        arduino.write(b'r,')
        arduino.write(bytes(left +"," + right, 'ascii'))
        arduino.write(b';')
        return 'turn right, %s, %s' % (left, right)
    finally:
        # 释放串口资源
        releaseLock()

@app.route('/left')
def turnLeft():
    global lastseq
    left = request.args.get('left', '255')
    right = request.args.get('right', '255')
    seq = int(request.args.get('seq'))
    # 获取往串口写数据的资源权限
    getLock()
    try:
        # 控制命令执行顺序，避免异步操作顺序异常
        if seq <= lastseq:
            return 'seq error'
        lastseq = seq
        arduino.write(b'l,')
        arduino.write(bytes(left +"," + right, 'ascii'))
        arduino.write(b';')
        return 'turn left, %s, %s' % (left, right)
    finally:
        # 释放串口资源
        releaseLock()

def getLock():
    '''
    获取写入串行数据资源
    '''
    global lock
    while lock != 0:
        sleep(10)
    lock = 1

def releaseLock():
    '''
    释放串行数据资源
    '''
    global lock
    lock = 0

if __name__ == '__main__':
    app.run(host='0.0.0.0', debug=True, port=8080) 