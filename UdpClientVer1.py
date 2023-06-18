import socket

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
