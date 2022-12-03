import socket

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
    print("Humidity: " + str(humidity) + "%%RH")
    print("Temperature: " + str(temperature) + "°C")