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
        humidity = list()
        rloc16 = int()
        eco2 = list()
        lights = list()
        pressure = list()
        sound = list()
        line_count = 0
        for row in reader:
            timeStamps.append(row[0])
            rloc16 = row[1]
            humidity.append(float(row[2]) / 1000)
            temperatures.append(float(row[3]) / 1000)
            lights.append(float(row[4]) / 100)
            pressure.append(float(row[6]) / 1000)
            eco2.append(float(row[7]))
            sound.append(float(row[9]) / 100)
            line_count += 1
        print(f'Processed {line_count} lines.')
    return timeStamps, rloc16, humidity, temperatures, lights, pressure, eco2, sound

def createPlot(x, rloc, measured, label):
    dates =[dt.fromtimestamp(float(ts)) for ts in x]
    datenums = md.date2num(dates)
    rloc16 = hex(int(rloc))
    plt.xlabel('Time')
    plt.ylabel('{}'.format(label))
    plt.title('Thunderboard Sense 2: '+ rloc16 +'')
    ax=plt.gca()
    xfmt = md.DateFormatter('%Y-%m-%d %H:%M:%S')
    ax.xaxis.set_major_formatter(xfmt)
    plt.gcf().autofmt_xdate()
    plt.margins(0.015, tight=True)
    values=[float(data) for data in measured]
    plt.plot(datenums, values)
    plt.savefig('server/static/{}'.format(f"{rloc16}{label}.png"))
    plt.clf()

# Main code starts here
UDP_IP = "0.0.0.0"
UDP_PORT = 12345
sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
sock.bind((UDP_IP, UDP_PORT))
while True:
    writeToFile()
    (x, rloc, humidity, temperatures, lights, pressure, eco2, sound) = readFromFile()
    createPlot(x, rloc, humidity, "Humidity");
    createPlot(x, rloc, temperatures, "Temperature")
    createPlot(x, rloc, lights, "Lights")
    createPlot(x, rloc, eco2, "CO2")
    createPlot(x, rloc, pressure, "Pressure")
    createPlot(x, rloc, sound, "Sound")


