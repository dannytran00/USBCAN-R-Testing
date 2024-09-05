#include <string>
#include <iostream>

#include "DJIR_SDK.h"
using namespace DJIR_SDK;

int main(void)
{
    std::cout << "###########################################" << std::endl;
    std::cout << "#                                         #" << std::endl;
    std::cout << "#          DJIR-SDK Test v1.0.0           #" << std::endl;
    std::cout << "#                                         #" << std::endl;
    std::cout << "###########################################" << std::endl;

    DJIRonin gimbal = DJIRonin();

    // Connect to DJI Ronin Gimbal
    gimbal.connect();

    // Select ABSOLUTE_CONTROL mode
    gimbal.set_move_mode(MoveMode::ABSOLUTE_CONTROL);

    // Move to center position (yaw = 0, roll = 0, pitch = 0) for 2000ms
    gimbal.move_to(90, 45, 30, 3000);

    gimbal.move_to(0,0,0,3000);

    int16_t yaw = 0;
    int16_t roll = 0;
    int16_t pitch = 0;
    gimbal.get_current_position(yaw, roll, pitch);
    std::cout <<"yaw = "<<yaw<<" roll = "<<roll<<" pitch = "<<pitch<<std::endl;


    std::cout << "Press any key to continie...";
    getchar();
    return 1;
}