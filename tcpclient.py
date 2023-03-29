import socket               # 导入 socket 模块

import socket

IP = '10.205.19.110'
#  IP = "127.0.0.1"
PORT = 11001

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((IP, PORT))
    s.sendall(b'Hello, world')
    data = s.recv(1024)

print('Received', repr(data))
