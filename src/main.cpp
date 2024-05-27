#include <Arduino.h>
#include <StepperDriver.h>
#include <Menu.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
//attach pin interrupt
void EmergencyStop();
//some variable
float duration;
Vector2 Size(10,20);
Vector2 StartPoint,EndPoint;
//some function to draw the menu
//since display can vary on devices, I decide not to put this part in header

//sh1106 define
#define OLED_RESET 4
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 1
#if (SH1106_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SH1106.h!");
#endif
//Joystick define
#define JOY_X_PIN 13
#define JOY_Y_PIN 12
#define JOY_Z_PIN 14
//Stepper Motor define
#define Motor_RX1 4
#define Motor_TX1 15
#define Motor_ADDR1 0xe0
#define Motor_RX2 16
#define Motor_TX2 17
#define Motor_ADDR2 0xe1
//
namespace SH1106{
    Adafruit_SH1106 display(OLED_RESET);
    void begin();
    void clear();
    void RenderPage(int);
}
namespace Menu{
    Page *Pages;
    int cursor_id = 0;
    int current_page_id = 0;
    int last_page_id = 0;
    Vector2 size(100,10);
    //modify this yourself
    void begin();
    void GoToPage(int);
    void HandleInput();
    void SliderStart();
    void SetSize();
};
namespace JoyStick{
    int x,y,z;
    __Enum::JoystickInput input;
    void begin();
    void TestReadJoyStick();
    void TestPrintJoyStick();
}

//StepperDriver::Motor *motor1 = new StepperDriver::Motor(Motor_RX1, Motor_TX1, Motor_ADDR1,1);
StepperDriver::Motor *motor2 = new StepperDriver::Motor(Motor_RX2, Motor_TX2, Motor_ADDR2,2);


void setup() {
    //initailize
    Serial.begin(115200);
    Serial.println("Starting");
    //motor1->begin();
    motor2->begin();
    JoyStick::begin();
    Menu::begin();
    SH1106::begin();
    SH1106::display.display();
    delay(1000);
}

void loop() {
    //Scan all Pins for new input
    JoyStick::TestReadJoyStick();
    JoyStick::TestPrintJoyStick();
    //Handle input from JoyStick
    Menu::HandleInput();
    //Handle input from motor driver
    //We don't handle the motor input in loop, instead we only read after we send message
    //Render
    SH1106::clear();
    //SH1106::RenderPage(Menu::current_page_id);//Menu::current_page_id);
    delay(200);
}

void EmergencyStop(){
    //motor1->W_EnableMotor(false);
    motor2->W_EnableMotor(false);
}

void JoyStick::begin(){
    pinMode(JOY_X_PIN,INPUT);
    pinMode(JOY_Y_PIN,INPUT);
    pinMode(JOY_Z_PIN,INPUT);
    x = 0;
    y = 0;
    z = 0;
}
void JoyStick::TestReadJoyStick(){
    x = analogRead(JOY_X_PIN);
    y = analogRead(JOY_Y_PIN);
    z = analogRead(JOY_Z_PIN);
    x -= 1960;
    y -= 1960;
    if(z == 0){
        input = __Enum::JoystickInput::Ok;
    }
    else if(abs(x) > 20 || abs(y) > 20){
        if(abs(x) > abs(y)){
            input = x > 0 ? __Enum::JoystickInput::Up : __Enum::JoystickInput::Down;
        }
        else
            input = y > 0 ? __Enum::JoystickInput::Right : __Enum::JoystickInput::Left;
    }
    else
        input = __Enum::JoystickInput::None;
}
void JoyStick::TestPrintJoyStick(){
    SH1106::display.clearDisplay();
    Serial.println("======JoyStick=====");
    Serial.printf("X: %d " , x);
    Serial.printf("Y: %d " , y);
    Serial.printf("Z: %d \n" , z);
    Serial.println("===================");
    SH1106::display.setCursor(0,0);
    SH1106::display.println(x);
    SH1106::display.setCursor(0,20);
    SH1106::display.println(y);
    SH1106::display.setCursor(0,40);
    SH1106::display.println(z);
    SH1106::display.display();
}


void SH1106::begin(){
    display.begin(SH1106_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
    display.setTextSize(1);
    display.setTextColor(WHITE);
}
void SH1106::clear(){
    display.clearDisplay();
}
void SH1106::RenderPage(int id){
    //display.write("Try to render...");
    for(int i = 0; i < Menu::Pages[id].elements_count; i++){
        Element *fig = Menu::Pages[id].figets[i];
        //check if the element is chosen
        bool chosen = Menu::Pages[id].figets[i]->fn_GetCursorOn();
        switch (Menu::Pages[id].figets[i]->GetType())
        { 
        case __Enum::Element_Type::none:
            display.write("Why doesn't it fucking work");
            break;
        case __Enum::Element_Type::Text:
            display.setCursor(fig->Coordinate.x + 1,fig->Coordinate.y + 1);
            display.setTextColor(WHITE);
            display.print(Menu::Pages[id].figets[i]->GetString());
            break;
        case __Enum::Element_Type::Button:
            display.drawRect(
                fig->Coordinate.x, fig->Coordinate.y,
                fig->Size.x, fig->Size.y,
                WHITE
            );
            display.fillRect(
                fig->Coordinate.x + 1, fig->Coordinate.y + 1,
                fig->Size.x - 2, fig->Size.y - 2,
                chosen ? WHITE : BLACK
            );
            display.setCursor(fig->Coordinate.x + 4,fig->Coordinate.y + 1);
            display.setTextColor(chosen ? BLACK : WHITE);
            display.print(Menu::Pages[id].figets[i]->GetString());//Menu::Pages[id].figets[i].GetString());
            break;
        case __Enum::Element_Type::NumPicker:
            display.drawRect(
                fig->Coordinate.x, fig->Coordinate.y,
                12 + 2,16 + 2,
                WHITE
            );
            display.fillRect(
                fig->Coordinate.x + 1, fig->Coordinate.y + 1,
                12 ,16 ,
                chosen ? WHITE : BLACK
            );
            display.setCursor(fig->Coordinate.x + 1, fig->Coordinate.y + 1);
            display.setTextColor(chosen ? BLACK : WHITE);
            display.print(Menu::Pages[id].figets[i]->GetVal(),DEC);
            break;
        case __Enum::Element_Type::Circle:
            display.fillCircle(fig->Coordinate.x, fig->Coordinate.y, 2, WHITE);
            break;
        default:
            display.setCursor(0,30);
            display.write("None of them fits");
            break;
        }
        /////////////
        
        //render differently
        //can't use std::is_same with switch
        //std::is_same is fucking useless   
    }
    display.display();
}

/// menu namespace ///
void Menu::begin(){
    Pages = new Page [7]{
        //Main Menu
        Page(0, 2, 2, new Element*[2]{
            new Button(Menu::size, Vector2(5,5), const_cast<char*>("Quick Start"), [](){
                Menu::GoToPage(1);
                //motor1->W_SetMaxTorque(0);
                motor2->W_SetMaxTorque(0);
                
                }),
            new Button(Menu::size, Vector2(5,20), const_cast<char*>("Settings"), [](){Menu::GoToPage(5);})
        }),
        
        //Set End Point
        Page(1, 3, 2, new Element*[3]{
            new Text(Menu::size, Vector2(5,5), const_cast<char*>("Set End Point")),
            new Button(Menu::size, Vector2(5,20), const_cast<char*>("Set"), [](){
                //motor1->W_Simple_Cmd(StepperDriver::ReadTypes::Encoder_RT_Pos);
                motor2->W_Simple_Cmd(StepperDriver::ReadTypes::Encoder_RT_Pos);
                //Warning! I think this part might contains potential risks
                delay(50);

                EndPoint.x = 0;//motor1->encoder_val;
                EndPoint.y = motor2->encoder_val;

                std::string s = "End: " + std::to_string(EndPoint.x) + ' ' + std::to_string(EndPoint.y);
                Pages[4].figets[1]->SetString(s); 

                Menu::GoToPage(2);
            }),
            new Button(Menu::size, Vector2(5,45), const_cast<char*>("Back"), [](){Menu::GoToPage(0);})
        }),
        //Set Start Point
        Page(2, 3, 2, new Element*[3]{
            new Text(Menu::size, Vector2(5,5), const_cast<char*>("Set Start Point")),
            new Button(Menu::size, Vector2(5,20), const_cast<char*>("Set"), [](){
                //motor1->W_Simple_Cmd(StepperDriver::ReadTypes::Encoder_RT_Pos);
                motor2->W_Simple_Cmd(StepperDriver::ReadTypes::Encoder_RT_Pos);
                //Warning! I think this part might contains potential risks
                delay(50);
                

                StartPoint.x = 0;//motor1->encoder_val;
                StartPoint.y = motor2->encoder_val;

                std::string s = "Start: " + std::to_string(StartPoint.x) + ' ' + std::to_string(StartPoint.y);
                Pages[4].figets[0]->SetString(s);

                Menu::GoToPage(3);
            }),
            new Button(Menu::size, Vector2(5,45), const_cast<char*>("Back"), [](){Menu::GoToPage(1);})
        }),
        //Set Duration
        Page(3, 10, 8, new Element*[10]{
            new Text(Menu::size, Vector2 (5,5), const_cast<char*>("Set Duration")),
            new BasicShape::Circle(Menu::size, Vector2(67,30)),
            new NumPicker(Menu::size, Vector2(5,15)),
            new NumPicker(Menu::size, Vector2(20,15)),
            new NumPicker(Menu::size, Vector2(35,15)),
            new NumPicker(Menu::size, Vector2(50,15)),
            //We'll Draw a dot here
            new NumPicker(Menu::size, Vector2(70,15)),
            new NumPicker(Menu::size, Vector2(85,15)),
            new Button(Menu::size, Vector2(5,35), const_cast<char*>("Confirm"), [](){
                duration = 0;
                duration += Pages[3].figets[2]->GetVal() * 1000;
                duration += Pages[3].figets[3]->GetVal() * 100;
                duration += Pages[3].figets[4]->GetVal() * 10;
                duration += Pages[3].figets[5]->GetVal();
                duration += Pages[3].figets[6]->GetVal() * 0.1f;
                duration += Pages[3].figets[7]->GetVal() * 0.01f;
                duration = duration == 0 ? 10 : duration;
                std::string d = std::to_string(duration);
                size_t dot_pos = d.find('.');
                std::string s = "Duration: " + d.substr(0, dot_pos + 1 + 2);
                Pages[4].figets[2]->SetString(s);
                //Write to the motor
                Menu::GoToPage(4);
            }),
            new Button(Menu::size, Vector2(5,50), const_cast<char*>("Back"), [](){Menu::GoToPage(2);})
        }),
        //Confirm 
        Page(4, 5, 2, new Element*[5]{
            new Text(Menu::size, Vector2(5,5), const_cast<char*>("Start: ")),
            new Text(Menu::size, Vector2(5,15), const_cast<char*>("End: ")),
            new Text(Menu::size, Vector2(5,25), const_cast<char*>("Duration: ")),
            new Button(Menu::size, Vector2(5,35), const_cast<char*>("Begin"), [](){
                Menu::GoToPage(6);
                Menu::SliderStart();
            }),
            new Button(Menu::size, Vector2(5,50), const_cast<char*>("Back"), [](){Menu::GoToPage(3);})
        }),
        //Settings
        Page(5,10, 8, new Element*[10]{
            new Text(Menu::size, Vector2 (5,5), const_cast<char*>("Set Size")),
            new BasicShape::Circle(Menu::size, Vector2(67,30)),
            new NumPicker(Menu::size, Vector2(5,15)),
            new NumPicker(Menu::size, Vector2(20,15)),
            new NumPicker(Menu::size, Vector2(35,15)),
            new NumPicker(Menu::size, Vector2(50,15)),
            //We'll Draw a dot here
            new NumPicker(Menu::size, Vector2(70,15)),
            new NumPicker(Menu::size, Vector2(85,15)),
            new Button(Menu::size, Vector2(5,35), const_cast<char*>("Confirm"), SetSize),
            new Button(Menu::size, Vector2(5,50), const_cast<char*>("Back"), [](){Menu::GoToPage(Menu::last_page_id);})
        }),
        Page(6,2,1, new Element*[2]{
            new Text(Menu::size, Vector2(5,5), const_cast<char*>("Time Left: ")),
            new Button(Menu::size, Vector2(5,35), const_cast<char*>("Stop"), [](){EmergencyStop();})
        })
    };
    Pages[0].figets[0]->fn_SetCursorOn(true);
}
void Menu::GoToPage(int id){
    last_page_id = current_page_id;
    current_page_id = id;
    cursor_id = Menu::Pages[current_page_id].elements_count - Menu::Pages[current_page_id].interactive_count;
    Menu::Pages[current_page_id].figets[cursor_id]->fn_SetCursorOn(true);
}
void Menu::HandleInput(){
    //First: de-choose the old chosen element
    int skip_ele_counts = Menu::Pages[current_page_id].elements_count - Menu::Pages[current_page_id].interactive_count;
        
    //Sick Trick: We put all the non-interactable element in the front, so 
    //cursor_id would be (element_cout - interactable_count) < cursor_id < element_count
    
    //Second: Check what type of element is chosen
    switch (JoyStick::input)
    {
    case __Enum::JoystickInput::Right:
        Menu::Pages[current_page_id].figets[cursor_id]->fn_SetCursorOn(false);
        Menu::cursor_id = Menu::cursor_id < Menu::Pages[Menu::current_page_id].elements_count - 1 ? Menu::cursor_id + 1 : skip_ele_counts;
        Menu::Pages[current_page_id].figets[cursor_id]->fn_SetCursorOn(true);
        break;
    case __Enum::JoystickInput::Left:
        Menu::Pages[current_page_id].figets[cursor_id]->fn_SetCursorOn(false);
        Menu::cursor_id = Menu::cursor_id > skip_ele_counts ? Menu::cursor_id - 1 : Menu::Pages[Menu::current_page_id].elements_count - 1;
        Menu::Pages[current_page_id].figets[cursor_id]->fn_SetCursorOn(true);
        break;
    case __Enum::JoystickInput::Up:
        switch (Menu::Pages[Menu::current_page_id].figets[cursor_id]->GetType())
        {
        case __Enum::NumPicker:{
            int a = Menu::Pages[Menu::current_page_id].figets[cursor_id]->GetVal();
            a = a < 9 ? a + 1 : 0;
            Menu::Pages[Menu::current_page_id].figets[cursor_id]->SetVal(a);
            break;
        }
        default:
            Menu::Pages[current_page_id].figets[cursor_id]->fn_SetCursorOn(false);
            Menu::cursor_id = Menu::cursor_id < Menu::Pages[Menu::current_page_id].elements_count - 1 ? Menu::cursor_id + 1 : skip_ele_counts;
            Menu::Pages[current_page_id].figets[cursor_id]->fn_SetCursorOn(true);
            break;
        }
        break;
    case __Enum::JoystickInput::Down:
        switch (Menu::Pages[Menu::current_page_id].figets[cursor_id]->GetType())
        {
        case __Enum::NumPicker:{
            int a = Menu::Pages[Menu::current_page_id].figets[cursor_id]->GetVal();
            a = a > 0 ? a - 1 : 9;
            Menu::Pages[Menu::current_page_id].figets[cursor_id]->SetVal(a);
            break;
        }
        default:
            Menu::Pages[current_page_id].figets[cursor_id]->fn_SetCursorOn(false);
            Menu::cursor_id = Menu::cursor_id > skip_ele_counts ? Menu::cursor_id - 1 : Menu::Pages[Menu::current_page_id].elements_count - 1;
            Menu::Pages[current_page_id].figets[cursor_id]->fn_SetCursorOn(true);
            break;
        }
        break;
    case __Enum::JoystickInput::Ok:
        if(Menu::Pages[Menu::current_page_id].figets[cursor_id]->GetType() == __Enum::Button){
            Menu::Pages[current_page_id].figets[cursor_id]->fn_SetCursorOn(false);
            Menu::Pages[Menu::current_page_id].figets[cursor_id]->OnClick();
        }
        break;
    }
    //3: 
}
void Menu::SliderStart(){
    //motor1->W_Simple_Cmd(StepperDriver::ReadTypes::Set_0);
    motor2->W_Simple_Cmd(StepperDriver::ReadTypes::Set_0);
    Serial.println("==========Set 0=========");
    
    //motor1->W_SetMaxTorque(0x0258);
    motor2->W_SetMaxTorque(0x0258);
    Serial.println("=========Set torque============");
    //delay(100);
    
    //motor1->W_Simple_Cmd(StepperDriver::ReadTypes::Goto_0);
    //motor2->W_Simple_Cmd(StepperDriver::ReadTypes::Goto_0);
    //motor1->Read_input();
    //motor2->Read_input();

    //encodert_val/65536 = how many turns
    //======================calculate speed========================
    //60 * turns/duration = rpm
    //Vrpm = (speed × 30000)/(Mstep × 200) 1.8度电机
    //in our case, speed = Vrpm x (Mstep x 200) / 30000
    
    int8_t speed1 = floorf((60 * ((EndPoint.x - StartPoint.x)  / 65536.0f / duration)) * (Mstep * 200) / 30000);
    int8_t speed2 = floorf((60 * ((EndPoint.y - StartPoint.y)  / 65536.0f / duration)) * (Mstep * 200) / 30000);
    Serial.println("======================");
    Serial.print("start Point");
    Serial.println(StartPoint.y);
    Serial.print("End Point");
    Serial.println(EndPoint.y);
    Serial.print("turns: ");
    Serial.println((EndPoint.y - StartPoint.y)  / 65536.0f);
    Serial.print("rpm: ");
    Serial.println(60 * ((EndPoint.y - StartPoint.y)  / 65536.0f / duration));
    Serial.print("Speed: ");
    Serial.println(floorf((60 * ((EndPoint.y - StartPoint.y)  / 65536.0f / duration)) * (Mstep * 200) / 30000));
    Serial.print("steps: ");
    Serial.println(abs(floorf(((EndPoint.y - StartPoint.y) / 65536.0f) * 3200)));
    Serial.println("======================");
    //======================calculate steps========================
    //360 degree = 3200 pulses => 1 pulse = 0.1125 degree
    float steps_1 = floorf(abs((EndPoint.x - StartPoint.x) / 65536.0f * (360/(1.8/Mstep))));
    float steps_2 = floorf(abs((EndPoint.y - StartPoint.y) / 65536.0f * (360/(1.8/Mstep))));
    //I hope this works
    
    //motor1->W_Drive_w_Steps(speed1, steps_1);
    motor2->W_Drive_w_Steps(speed2, steps_2);


    unsigned long start_time = millis();
    float time_left = duration - (millis() - start_time)/1000;
    while(time_left > 0){
        time_left = duration - (millis() - start_time)/1000  ;
        Pages[6].figets[0]->SetString("Time Left: " + std::to_string(time_left));
        SH1106::RenderPage(6);
        SH1106::clear();
    }
    //motor1->W_SetMaxTorque(0);
    //motor2->W_SetMaxTorque(0);
    //
    //motor2->Read_input();
    Menu::GoToPage(0);
}
void Menu::SetSize(){};