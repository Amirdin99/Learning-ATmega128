from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from threading import Thread
from time import sleep
from serial import Serial
from random import randrange

class Galaga(QWidget):
    def __init__(self):
        super().__init__()
        self.resize(500,900)
        self.me = QRect(200,750, 100,100)
        self.bul = QRect(-100,-100, 10, 10)
        self.imgs = ['EnemyBee.bmp','EnemyButterfly.bmp', 'EnemyShip.bmp']
        self.ships = [
            (QRect(15,  -randrange(100, 400), 50,50),  randrange(0, 3)),
            (QRect(140, -randrange(100, 400), 50,50), randrange(0, 3)),
            (QRect(265, -randrange(100, 400), 50,50), randrange(0, 3)),
            (QRect(390, -randrange(100, 400), 50,50), randrange(0, 3))
        ]
        self.color = Qt.red
        self.msg = ''
        th = Thread(target=self.readThread, args=())
        th.daemon = True
        th.start()

    def paintEvent(self, e):
        dc = QPainter(self)
        dc.fillRect(0,0,500,900, Qt.black)

        img = QImage('ship.bmp')
        dc.drawImage(self.me, img)
        dc.fillRect(self.bul, self.color)
        
        for ship, img in self.ships:
            img = QImage(self.imgs[img])
            dc.drawImage(ship, img)

        font = dc.font()
        font.setBold(True)
        font.setFamily('Arial')
        font.setPointSize(30)
        dc.setPen(Qt.red)
        dc.setFont(font)
        dc.drawText(QRect(0,0,500,900), Qt.AlignCenter, self.msg)

        dc.end()

    def readThread(self):
        ser = Serial('COM3')
        ser.flush()
        i = 0
        rgb = 'rgb'
        color = [Qt.red, Qt.green, Qt.blue]
        while True:
            data = ser.read(2).decode('utf-8')
            print(data)

            if data[0] == '1' and self.me.left() - 10 >= 0:
                self.me.moveTo(self.me.left() - 10, self.me.top())
            elif data[0] == '2' and self.me.left() + 10 <= 400:
                self.me.moveTo(self.me.left() + 10, self.me.top())
            
            self.bul.moveTo(self.bul.left(), self.bul.top() - 30)
            if data[1] == '1' and self.bul.top() < 0:
                self.bul.setRect(self.me.left() + 50,self.me.top(),10,10)
                i += 1
                self.color = color[i % 3]
                ser.write(rgb[i % 3].encode())
        
            for ship, _ in self.ships:
                ship.moveTo(ship.left(), ship.top() + 10)

                if ship.contains(self.bul):
                    self.ships.remove((ship, _))
                    self.bul.moveTo(-100, -100)

                if (ship.contains(self.me.center())):
                    self.msg = 'You Died!!'
                    self.update()
                    return

                if (ship.top() > 1000):
                    ship.moveTo(ship.left(), -randrange(50, 300))
            
            if len(self.ships) == 0:
                self.msg = 'You Win!!'
                self.update()
                return

            self.update()

if __name__ == '__main__':
    import sys
    app = QApplication(sys.argv)
    ui = Galaga()
    ui.show()
    sys.exit(app.exec_())