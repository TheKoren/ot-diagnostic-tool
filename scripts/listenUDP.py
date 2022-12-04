import socket
from datetime import datetime
import os


UDP_IP = "0.0.0.0"
UDP_PORT = 12345

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
sock.bind((UDP_IP, UDP_PORT))

while True:
    data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
    strList = data.decode('utf8', 'strict').split();
    humidity = int(strList[2]) / 1000
    temperature = int(strList[6]) / 1000
    now = datetime.now()
    current_time = now.strftime("%m/%d %H:%M:%S")
    file = open('data.txt', 'a')
    file.write(str(current_time) + "," + str(temperature) + "," + str(humidity) + "\n")
    file.close()
    print(str(current_time) + "\t" + str(temperature) + "\t" + str(humidity) + "\n")