#include "StepperDriver.h"

void StepperDriver::Motor::begin(){
    if(this->Serial_num == 1){
        Serial1.begin(38400);
        Serial1.setPins(this->RX_Pin,this->TX_Pin);
        this->W_EnableMotor(true);
        //this->Read_input();
        this->W_SetMaxTorque(0x256);
    }
    else if(this->Serial_num == 2){
        Serial2.begin(38400);
        Serial2.setPins(this->RX_Pin,this->TX_Pin);
        this->W_EnableMotor(true);
        this->W_SetMaxTorque(0x256);
    }
}

void StepperDriver::Motor::Read_input(){
    uint8_t *read_msg;
    bool new_input = false;
    while(new_input == false){
        if(Serial_num == 1){
            if(Serial1.available()){
                if(Serial1.read() == this->UART_Addr){
                    new_input = true;
                }
            }
        }
        else if(Serial_num == 2){
            if(Serial2.available()){
                if(Serial2.read() == this->UART_Addr){
                    new_input = true;
                }
            }
        }
    }
    new_cmd = false;
    ////////////////////////Assign how many bytes should we read////////////
    int counts; 
    switch (this->last_type)
    {
    case StepperDriver::ReadTypes::Encoder_RT_Pos: case StepperDriver::ReadTypes::Pulse:
        counts = 6;
        break;
    case StepperDriver::ReadTypes::AngleOffset:
        counts = 4;
    case StepperDriver::ReadTypes::Drive_W_Steps: case StepperDriver::ReadTypes::Clogged_or_Not:
    case StepperDriver::ReadTypes::EnableMotor: case StepperDriver::ReadTypes::Set_0:
    case StepperDriver::ReadTypes::Goto_0: case StepperDriver::ReadTypes::Set_MaxTorque:
        counts = 3;
        break;
    }
    ///////////////////////Reading the message///////////
    read_msg = new uint8_t [counts];
    read_msg[0] = this->UART_Addr;
    if(this->Serial_num == 1){
        for(int i = 1;i < counts; i++)
            if(Serial1.available()){
                read_msg[i] = Serial1.read();
            }
    }
    else {
        for(int i = 1;i < counts; i++)
            if(Serial2.available()){
                read_msg[i] = Serial2.read();
            }
    }
    ////////////////////////Do sth after reading the message////////////////
    switch (this->last_type)
    {
    case StepperDriver::ReadTypes::Encoder_RT_Pos:{
        int32_t pos = (read_msg[1] << 24) | (read_msg[2] << 16) | (read_msg[3] << 8) | read_msg[4];
        this->encoder_val = pos;
        break;
    }
    case StepperDriver::ReadTypes::AngleOffset:{
        int pos = (read_msg[1] << 24) + (read_msg[2] << 16) + (read_msg[3] << 8) + read_msg[4];
        this->encoder_val = pos;
        break;
    }
    case StepperDriver::ReadTypes::Drive_W_Steps:
        
        break;
    case StepperDriver::ReadTypes::Clogged_or_Not:
        
        break;
    case StepperDriver::ReadTypes::EnableMotor:
        
        break;
    case StepperDriver::ReadTypes::Pulse:
        
        break;
    case StepperDriver::ReadTypes::Set_0:
        
        break;
    case StepperDriver::ReadTypes::Goto_0:
        
        break;
    }
    ///////////////////////////////
        Serial.println("==============new input==================");
        for(int i = 0;i < counts; i++){
            Serial.print("0x");
            Serial.print(read_msg[i], HEX);
            Serial.print(' ');
        }
        Serial.print("\n=========================================");
    ///////////////////////////////
    int rchk_byte = read_msg[counts - 1];// we don't read the last byte
    int chk = 0;
    for(int i = 0;i < counts - 1;i++){
        chk += read_msg[i];
    }
    chk = chk & 0xff;
    //read failed
    if(rchk_byte == chk){
        Serial.println("Chk_Byte works");
        //alert();
        //make sure msg is blank next time
        for(int i = 1;i < msg_len;i++){ // i = 1 because [0] is address
            msg[i] = '\0';
        }
    }
    else{
        Serial.println("Fuck");
        Serial.print("Chk Byte should be ");
        Serial.println(chk,HEX);
        //send the msg one more time
        this->Write_2_Driver(msg);
    }
    delete [] read_msg;
}

void StepperDriver::Motor::W_Simple_Cmd(ReadTypes type){
    new_cmd = true;
    this->last_type = type;
    this->msg[1] = type;
    if(type == Set_0 || type == Goto_0){
        this->msg[2] = 0x00;
        msg_len = 3;
    }
    else{
        msg_len = 2;
    }
        
    Add_CheckByte(this->msg, this->msg_len);
    Write_2_Driver(this->msg);
}
void StepperDriver::Motor::W_Drive_w_Steps(int8_t speed,unsigned int steps){
    msg_len = 7;
    this->msg[1] = StepperDriver::ReadTypes::Drive_W_Steps;
    this->msg[2] = speed;
    this->msg[3] = (steps & 0xff000000) >> 24;
    this->msg[4] = (steps & 0xff0000) >> 16;
    this->msg[5] = (steps & 0xff00) >> 8;
    this->msg[6] = steps & 0xff;
    Add_CheckByte(msg,msg_len);
    Write_2_Driver(msg);
    this->last_type = StepperDriver::ReadTypes::Drive_W_Steps;
    //
    Serial.println("Sent ");
}
void StepperDriver::Motor::W_EnableMotor(bool enable){
    new_cmd = true;
    this->msg[1] = StepperDriver::EnableMotor;
    this->msg[2] = enable ? 0x01 : 0x00;
    msg_len = 3;
    Add_CheckByte(msg,msg_len);
    Write_2_Driver(msg);
    this->last_type = StepperDriver::ReadTypes::EnableMotor;
}
void StepperDriver::Motor::W_SetMaxTorque(int16_t max){
    new_cmd = true;
    this->msg[1] = StepperDriver::ReadTypes::Set_MaxTorque;
    this->msg[2] = (max & 0xff00) >> 8;
    this->msg[3] = max & 0xff;
    msg_len = 4;
    Add_CheckByte(msg,msg_len);
    Write_2_Driver(msg);
    this->last_type = StepperDriver::ReadTypes::Set_MaxTorque;
}
void StepperDriver::Motor::Write_2_Driver(u_int8_t *msg){
    new_cmd = true;
    if(Serial_num == 1){
        Serial.println("==========Sent==========");
        for(int i = 0;i < msg_len; i++){
            Serial1.write(msg[i]);
            Serial.print("0x");
            Serial.print(msg[i],HEX);
            Serial.print(' ');
        }
        Serial.println("\n=======================");
    }
    else if(Serial_num == 2){
        Serial.println("==========Sent==========");
        for(int i = 0;i < msg_len; i++){
            Serial2.write(msg[i]);
            Serial.print("0x");
            Serial.print(msg[i],HEX);
            Serial.print(' ');
        }
        Serial.println("\n=======================");
    }
    //check if the data is recieved correctly
    delay(50);
    this->Read_input();
}
void StepperDriver::Add_CheckByte(uint8_t *msg,int &len){
    uint32_t tchk_byte = 0;
    for(int i = 0;i < len;i++){
        tchk_byte += msg[i];
    }
    msg[len] = tchk_byte & 0xff;
    len++;
    //msg[len + 1] = '\0';
}

    