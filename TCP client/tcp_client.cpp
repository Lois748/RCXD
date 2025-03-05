#include <iostream>
#include <sstream>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>  // For inet_pton()
#include "Motor_calc.cpp"   // Include the RC-CAR.cpp file to access global variables
#include <thread>

#pragma comment(lib, "ws2_32.lib")  // Link with Winsock library

#define SERVER_IP "192.168.0.246"  // Replace with ESP32's IP
#define SERVER_PORT 1234
#define INTERVAL_MS 1000  // Interval between data sends (in milliseconds)

int main() {
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return -1;
    }

    // Create socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed\n";
        WSACleanup();
        return -1;
    }

    // Server address setup
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    // Connect to ESP32 server
    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed\n";
        closesocket(sock);
        WSACleanup();
        return -1;
    }

    // Update motor values in a separate thread
    std::thread motorThread(updateMotorValues); // This will run the motor update loop

    // Sending data continuously
    while (true) {
        // Format the message to send
        char message[50];
        snprintf(message, sizeof(message), "Motor1: %.2f, Motor2: %.2f\n", global_motor_left, global_motor_right);

        // Send the message
        send(sock, message, strlen(message), 0);
        std::cout << "Sent: " << message << "\n";

        // Wait for a specified time interval before sending again
        Sleep(INTERVAL_MS);  // Sleep for 1 second (INTERVAL_MS)
    }

    // Cleanup (though this won't be reached in this infinite loop)
    closesocket(sock);
    WSACleanup();

    return 0;
}