import socket
import struct

UDP_IP = "127.0.0.1"  # Địa chỉ IP của UDP server
UDP_PORT = 8888
BUFFER_SIZE = 1024

# Tạo socket UDP
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Gửi yêu cầu lấy ảnh từ server
sock.sendto(b"GetImage", (UDP_IP, UDP_PORT))

# Nhận kích thước tệp ảnh từ server
data, addr = sock.recvfrom(4)
imageSize = struct.unpack('!I', data)[0]

# Mở tệp ảnh để ghi dữ liệu nhận được
imageFile = open("received_image.png", "wb")

totalReceived = 0
while totalReceived < imageSize:
    # Nhận dữ liệu từ server
    data, addr = sock.recvfrom(BUFFER_SIZE)
    totalReceived += len(data)

    # Ghi dữ liệu vào tệp ảnh
    imageFile.write(data)
    # Đóng tệp ảnh và socket
imageFile.close()
sock.close()

