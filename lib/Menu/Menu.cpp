#include <Menu.h>

void Text::SetString(const char* a){
    this->DisplayText = a;
}
void Text::SetString(const std::string &a){
    this->DisplayText = a;
}
/// Definition of Elements and its child ///
void Button::OnClick(){
    if(this->OnClickFunc != nullptr){
        this->OnClickFunc();
    }
}
char* Button::GetString() {
    return this->DisplayText;
}
bool Button::fn_GetCursorOn(){return CursorOn;}
void Button::fn_SetCursorOn(bool on){CursorOn = on;}
bool NumPicker::fn_GetCursorOn(){return CursorOn;}
void NumPicker::fn_SetCursorOn(bool on){CursorOn = on;}
/// Definition of Page ///
/*
Page::Page(int a,int b,int c, Element *elements){
    this->id = a;
    this->elements_count = b;
    this->interactive_count = c;
    for(int i = 0;i < elements_count;i++){
        figets[i] = elements[i];
    }

}
*/
Page::~Page(){
    delete [] figets;
}
