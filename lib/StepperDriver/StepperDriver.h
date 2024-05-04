#pragma once
#include <Arduino.h>

#define STM_A_RX_PIN
#define STM_A_TX_PIN
#define STM_B_RX_PIN
#define STM_B_TX_PIN
#define STM_A_Addr
#define STM_B_Addr

//what is the current insturction reading
typedef enum ReadTypes{
    Encoder = 0x30,
    Pulse = 0x33,
    AngleOffset = 0x39,
    EnableMotor = 0x3a,
    Clogged_or_Not = 0x3e
};
class Motor{
    public:
    int RX_Pin;
    int TX_Pin;
    int UART_Addr;
    char msg[10] = {UART_Addr};
};

//not sure if it's a good idea to put a function that uses serial write in here
void Write_2_Driver(char *msg);
void Add_CheckByte(char *msg);
void Read_input(char *read_msg);

//when the msg is simply changing the char[1]
void Simple_Read(Motor* motor,ReadTypes type);

void W_Drive_w_Steps(Motor* motor,int speed,int steps);
void W_EnableMotor(Motor *motor,bool enable);
