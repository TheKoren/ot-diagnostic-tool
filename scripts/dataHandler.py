import socket
import datetime as dt
import matplotlib.pyplot as plt
import matplotlib.dates as md
from datetime import datetime as dt
import csv

def writeToFile():
    data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
    data = data.decode('utf8', 'strict').split(',');
    data.insert(0, dt.now().timestamp())
    print(data)
    with open('data.csv', 'a', newline='', encoding='utf-8') as fp:
        writer = csv.writer(fp)
        writer.writerow(data)

def readFromFile():
    with open('data.csv', 'r') as fp:
        reader = csv.reader(fp, delimiter=',')
        timeStamps = list()
        temperatures = list()
        rloc16 = int()
        line_count = 0
        for row in reader:
            timeStamps.append(row[0])
            rloc16 = row[1]
            temperatures.append(row[3])
            line_count += 1
        print(f'Processed {line_count} lines.')
    return timeStamps, rloc16, temperatures

UDP_IP = "0.0.0.0"
UDP_PORT = 12345

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
sock.bind((UDP_IP, UDP_PORT))

while True:
    writeToFile()
    (x, rloc, y) = readFromFile()
    dates =[dt.fromtimestamp(float(ts)) for ts in x]
    datenums = md.date2num(dates)
    rloc16 = hex(int(rloc))
    plt.xlabel('Time')
    plt.ylabel('Temperature')
    plt.title('Thunderboard Sense 2: '+ rloc16 +'')
    ax=plt.gca()
    xfmt = md.DateFormatter('%Y-%m-%d %H:%M:%S')
    ax.xaxis.set_major_formatter(xfmt)
    values=[float(data) / 1000 for data in y]
    plt.plot(datenums, values)
    plt.gcf().autofmt_xdate()
    plt.margins(0.015, tight=True)
    plt.savefig('server/static/{}'.format(f"{rloc16}.png"))