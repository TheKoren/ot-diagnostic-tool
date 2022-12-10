import socket
from datetime import datetime
import csv


UDP_IP = "0.0.0.0"
UDP_PORT = 12345

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
sock.bind((UDP_IP, UDP_PORT))

while True:
    data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
    data = data.decode('utf8', 'strict').split(',');
    data.insert(0, datetime.now().timestamp())
    print(data)
    with open('data.csv', 'a', newline='', encoding='utf-8') as fp:
        writer = csv.writer(fp)
        writer.writerow(data)