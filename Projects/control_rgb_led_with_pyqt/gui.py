from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *
from serial import Serial

ser = Serial('COM3')

class Main(QWidget):
    def __init__(self):
        super().__init__()
        
        vbox = QVBoxLayout()
        self.red = QLineEdit()
        vbox.addWidget(self.red)
        red_label = QLabel('RED')
        red_label.setAlignment(Qt.AlignCenter)
        vbox.addWidget(red_label)

        self.green = QLineEdit()
        vbox.addWidget(self.green)
        green_label = QLabel('GREEN')
        green_label.setAlignment(Qt.AlignCenter)
        vbox.addWidget(green_label)

        self.blue = QLineEdit()
        vbox.addWidget(self.blue)
        blue_label = QLabel('BLUE')
        blue_label.setAlignment(Qt.AlignCenter)
        vbox.addWidget(blue_label)

        self.btn = QPushButton('Send')
        self.btn.clicked.connect(self.send)
        vbox.addWidget(self.btn)

        self.setLayout(vbox)
    
    def send(self):
        arr = bytes([int(self.red.text()), int(self.green.text()), int(self.blue.text())])
        print(arr)
        ser.write(arr)

if __name__ == '__main__':
    import sys
    app = QApplication(sys.argv)
    ui = Main()
    ui.show()
    sys.exit(app.exec_())

    
