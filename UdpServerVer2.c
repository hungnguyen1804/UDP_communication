#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8888
#define MAX_BUFFER_SIZE 1024

int main() {
    int sockfd;
    char buffer[MAX_BUFFER_SIZE];
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size;

    // Tạo socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Error in socket");
        exit(1);
    }

    // Cấu hình địa chỉ server
    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Gắn địa chỉ server vào socket
    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Error in bind");
        exit(1);
    }

    addr_size = sizeof(clientAddr);

    // Nhận yêu cầu từ client
    ssize_t dataSize;
    dataSize = recvfrom(sockfd, buffer, MAX_BUFFER_SIZE, 0, (struct sockaddr*)&clientAddr, &addr_size);
    if (dataSize < 0) {
        perror("Error in recvfrom");
        exit(1);
    }

    // Gửi kích thước của tệp ảnh cho client
    FILE* imageFile = fopen("/home/hungnguyen/Downloads/robot_arm.png", "rb");
    if (!imageFile) {
        perror("Error opening image file");
        exit(1);
    }
    fseek(imageFile, 0, SEEK_END);
    size_t imageSize = ftell(imageFile);
    rewind(imageFile);
    sendto(sockfd, &imageSize, sizeof(imageSize), 0, (struct sockaddr*)&clientAddr, addr_size);

    // Gửi dữ liệu của tệp ảnh cho client
    size_t totalSent = 0;
    while (totalSent <= imageSize) {
        // Đọc dữ liệu từ tệp ảnh
        size_t bytesRead = fread(buffer, 1, MAX_BUFFER_SIZE, imageFile);
        totalSent += bytesRead;

        // Gửi dữ liệu cho client
        if (sendto(sockfd, buffer, bytesRead, 0, (struct sockaddr*)&clientAddr, addr_size) < 0) {
            perror("Error in sendto");
            exit(1);
        }
    }

    // Đóng tệp ảnh và socket
    fclose(imageFile);
    close(sockfd);

    return 0;
}
