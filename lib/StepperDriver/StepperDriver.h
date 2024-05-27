#ifndef _Stepper_Driver
#define _Stepper_Driver

#include <Arduino.h>

#define STM_A_RX_PIN
#define STM_A_TX_PIN
#define STM_B_RX_PIN
#define STM_B_TX_PIN
#define STM_A_Addr
#define STM_B_Addr

#define Mstep 256


namespace StepperDriver{
    //what is the current insturction reading
    enum ReadTypes{
        Encoder_RT_Pos = 0x36,
        Pulse = 0x33,
        AngleOffset = 0x39,
        EnableMotor = 0xf3,
        Drive_W_Steps = 0xfd,
        Clogged_or_Not = 0x3e,
        Set_0 = 0x91,
        Goto_0 = 0x94,
        Set_MaxTorque = 0xa5
    };
    class Motor{
        public:
        //(int rx_pin, int tx_pin, char addr, int serial_num)
        Motor(int a,int b,char c,int d): RX_Pin(a),TX_Pin(b),UART_Addr(c), Serial_num(d){
            encoder_val = 0;
            new_cmd = false;
        };
        int RX_Pin;
        int TX_Pin;
        char UART_Addr;
        int encoder_val;
        ReadTypes last_type;
        uint8_t msg[10] = {UART_Addr};
        int msg_len = 0;
        int Serial_num;
        bool new_cmd;

        void begin();
        void Read_input();
        //not sure if it's a good idea to put a function that uses serial write in here
        void Write_2_Driver(uint8_t *msg);
        //when the msg is simply changing the char[1]
        void W_Simple_Cmd(ReadTypes type);
        void W_Drive_w_Steps(int8_t speed,unsigned int steps);
        void W_EnableMotor(bool enable);
        // min: 0, max: 0x4b0
        void W_SetMaxTorque(int16_t);
    };
    void Add_CheckByte(uint8_t *msg,int &len);    
}
#endif