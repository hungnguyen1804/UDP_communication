#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_SIZE 1024


void receive_image(int socket, struct sockaddr_in client_address, socklen_t address_length) {
    char buffer[BUFFER_SIZE];
    
    // Nhận kích thước tệp ảnh
    memset(buffer, 0, BUFFER_SIZE);
    ssize_t bytes_received = recvfrom(socket, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&client_address, &address_length);
    
    int file_size = atoi(buffer);
    printf("Received image size: %d bytes\n", file_size);
    
    // Nhận dữ liệu ảnh
    memset(buffer, 0, BUFFER_SIZE);
    FILE* file = fopen("received_image.jpg", "wb");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }
    
    ssize_t total_bytes_received = 0;
    while (total_bytes_received < file_size) {
        bytes_received = recvfrom(socket, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&client_address, &address_length);
        fwrite(buffer, 1, bytes_received, file);
        total_bytes_received += bytes_received;
    }
    
    fclose(file);
    printf("Image received and saved as 'received_image.jpg'.\n");
}

int main() {
	// Tạo socket UDP
	int server_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (server_socket < 0) {
		printf("Error creating socket.\n");
		return -1;
	}
	
	// Thiết lập địa chỉ server
	struct sockaddr_in server_address, client_address;
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(12345);
	
	// Gán địa chỉ server cho socket
	if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
		printf("Error binding socket.\n");
		return -1;
	}
	
	printf("Server listening on port 12345...\n");
	
	// Nhận dữ liệu ảnh từ client
	socklen_t address_length = sizeof(client_address);
	receive_image(server_socket, client_address, address_length);		
	
	// Đóng socket
	close(server_socket);
    
    
    return 0;
}
