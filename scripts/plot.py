import matplotlib.pyplot as plt
import matplotlib.dates as md
import datetime as dt
import csv

def readFile():
    with open('scripts/data.csv', 'r') as fp:
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

(x, rloc, y) = readFile()
dates =[dt.datetime.fromtimestamp(float(ts)) for ts in x]
datenums = md.date2num(dates)
plt.xlabel('Time')
plt.ylabel('Temperature')
plt.title('Thunderboard Sense 2: '+ hex(int(rloc)) +'')
ax=plt.gca()
xfmt = md.DateFormatter('%Y-%m-%d %H:%M:%S')
ax.xaxis.set_major_formatter(xfmt)
values=[float(data) / 1000 for data in y]
plt.plot(datenums, values)
plt.gcf().autofmt_xdate()
plt.margins(0, tight=True)
plt.savefig('server/static/{}'.format("data.png"))