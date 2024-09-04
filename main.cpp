#include <string>
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include "DJIR_SDK.h"

using namespace DJIR_SDK;

void print_banner() {
    std::cout << "###########################################" << std::endl;
    std::cout << "#                                         #" << std::endl;
    std::cout << "#    DJIR-SDK Streaming Test v1.0.0       #" << std::endl;
    std::cout << "#                                         #" << std::endl;
    std::cout << "###########################################" << std::endl;
}

void stream_data(DJIRonin &gimbal, std::ofstream &file, std::atomic<bool> &is_streaming) {
    std::cout << "Streaming data. Press Ctrl+C to stop." << std::endl;

    while (is_streaming) {
        int16_t yaw, roll, pitch;
        if (gimbal.get_current_position(yaw, roll, pitch)) {
            file << "Yaw: " << yaw
                 << ", Roll: " << roll
                 << ", Pitch: " << pitch << std::endl;
        } else {
            std::cerr << "Failed to get current position." << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main(void) {
    print_banner();

    DJIRonin gimbal;

    // Connect to DJI Ronin Gimbal
    bool is_connected = gimbal.connect();
    if (!is_connected) {
        std::cerr << "Failed to connect to DJI Ronin Gimbal!" << std::endl;
        //return 1; // Return an error code
    } else {
        std::cout << "Successfully connected to DJI Ronin Gimbal." << std::endl;
    }

    // Open a file for writing the streamed data
    std::ofstream data_file("streamed_data.txt");
    if (!data_file.is_open()) {
        std::cerr << "Failed to open file for writing." << std::endl;
        //return 1;
    }

    std::atomic<bool> is_streaming(true);
    
    // Start streaming data in a separate thread
    std::thread streaming_thread(stream_data, std::ref(gimbal), std::ref(data_file), std::ref(is_streaming));
    
    // Run the main loop to keep the program running
    std::cout << "Press Ctrl+C to stop streaming and exit..." << std::endl;
    int count = 0;
    int maxCount = 10000;
    while (count <= maxCount) {
        if (count > maxCount) {
            break;
        }
        // Keep the program running and responsive
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        count += 200;
    }

    // Graceful shutdown
    is_streaming = false;  // Stop streaming
    streaming_thread.join(); // Wait for the streaming thread to finish

    // Disconnect and close the file
    gimbal.disconnect();
    data_file.close();

    std::cout << "Disconnected from DJI Ronin Gimbal." << std::endl;
    std::cout << "Data saved to streamed_data.txt" << std::endl;
    std::cout << "Press any key to exit...";
    std::cin.ignore();
    std::cin.get();
    return 0;
}