#include "StepperDriver.h"

void Write_2_Driver(char *msg){

    //male sure msg is blank next time
    for(int i = 1;i < strlen(msg);i++){ // i = 1 because [0] is address
        msg[i] = '\0';
    }
}
void Add_CheckByte(char *msg){
    int len = strlen(msg);
    int tchk_byte = 0;
    for(int i = 0;i < len;i++){
        tchk_byte += (msg[i] & 0xff);
    }
    tchk_byte = tchk_byte & 0xff;
    msg[len] = tchk_byte;
    //msg[len + 1] = '\0';
}
void Read_input(char *read_msg){
    int len = strlen(read_msg);
    int rchk_byte = read_msg[len - 1];
    int chk = 0;
    for(int i = 0;i < len - 1;i++){
        chk += (read_msg[i] & 0xff);
    }
    chk = chk & 0xff;
    //read failed
    if(rchk_byte != chk){
        //alert();
    }
}

void Simple_Read(Motor* motor, ReadTypes type){
    motor->msg[1] = type;
    Add_CheckByte(motor->msg);
    Write_2_Driver(motor->msg);
}
void W_Drive_w_Steps(Motor* motor,int speed,int steps){
    motor->msg[1] = 0xfd;
}
void W_EnableMotor(Motor *Motor,bool enable){

}


    