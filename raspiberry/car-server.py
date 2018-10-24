from flask import Flask
from flask import render_template
from flask import request
from serial import Serial
import time

arduino = Serial('/dev/ttyAMA0', 9600, timeout=1)
app = Flask(__name__)

@app.route('/')
def index():
    return render_template('index.html') 

@app.route('/controller')
def controller2():
    return render_template('controller.html')

@app.route('/forward')
def forward():
    left = request.args.get('left', '255')
    right = request.args.get('right', '255')
    arduino.write(b'f,')
    arduino.write(bytes(left +"," + right, 'ascii'))
    arduino.write(b';')
    return 'forward, %s, %s' % (left, right)

@app.route('/brake')
def brake():
    arduino.write(b's;')
    return 'brake'

@app.route('/backward')
def backward():
    left = request.args.get('left', '255')
    right = request.args.get('right', '255')
    arduino.write(b'b,')
    arduino.write(bytes(left +"," + right, 'ascii'))
    arduino.write(b';')
    return 'backward, %s, %s' % (left, right)

@app.route('right')
def turnRight():
    left = request.args.get('left', '255')
    right = request.args.get('right', '255')
    arduino.write(b'r,')
    arduino.write(bytes(left +"," + right, 'ascii'))
    arduino.write(b';')
    return 'turn right, %s, %s' % (left, right)

@app.route('left')
def turnLeft():
    left = request.args.get('left', '255')
    right = request.args.get('right', '255')
    arduino.write(b'l,')
    arduino.write(bytes(left +"," + right, 'ascii'))
    arduino.write(b';')
    return 'turn left, %s, %s' % (left, right)

if __name__ == '__main__':
    app.run(host='0.0.0.0', debug=True, port=8080) 