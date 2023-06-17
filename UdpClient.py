import socket

def receive_image(server_address, server_port, save_filename):
    # Tạo socket UDP
    #server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    
    # Gắn địa chỉ và cổng của server với socket
    #server_socket.bind((server_address, server_port))
    
    # Nhận kích thước tệp ảnh
   # buffer_size = 1024
   # buffer = server_socket.recv(buffer_size)
    file_size = int(buffer.decode())
    print(f"Received image size: {file_size} bytes")
    
    # Nhận dữ liệu ảnh
    received_size = 0
    received_data = b""
    while received_size < file_size:
        data, _ = server_socket.recvfrom(buffer_size)
        received_data += data
        received_size += len(data)
    
    # Lưu dữ liệu ảnh vào tệp
    with open(save_filename, "wb") as file:
        file.write(received_data)
    
    print(f"Image received and saved as '{save_filename}'")


def send_image(filename, server_address, server_port):
    # Tạo socket UDP
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    
    # Mở tệp ảnh và đọc dữ liệu
    with open(filename, 'rb') as file:
        image_data = file.read()
    
    # Gửi kích thước của tệp ảnh trước
    file_size = len(image_data)
    client_socket.sendto(str(file_size).encode(), (server_address, server_port))
    
    # Chia nhỏ dữ liệu ảnh thành các phần nhỏ hơn và gửi lần lượt
    chunk_size = 1024  # Kích thước phần nhỏ
    offset = 0
    while offset < file_size:
        chunk = image_data[offset:offset+chunk_size]
        client_socket.sendto(chunk, (server_address, server_port))
        offset += chunk_size
    
    # Đóng socket
    client_socket.close()

# Thay đổi địa chỉ và cổng của server ở đây
server_address = '127.0.0.1'
server_port = 12345

# Thay đổi tên tệp ảnh và đường dẫn ở đây
image_filename = 'UAV.jpg'

# Gửi tệp ảnh đến server
while True:
    send_image(image_filename, server_address, server_port)
    break
