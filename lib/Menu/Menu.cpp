#include <Menu.h>

Menu::Menu(float &Du,Vector2 &Size){
    this->duaration = &Du;
    this->size = &Size;
}

void Menu::fn_SetPage(Page page){
    this->NewPage(page);
    this->CurrentPage = page;
}
void Menu::fn_Back(){
    this->fn_SetPage(this->LastPage);
}

//To Render the current page
void Menu::DrawMenu(){
    switch(this->CurrentPage){
        case Page::MainMenu:
            ///
            //Add a logo bmp here
            ///
        break;
        case Page::QuickStart:
        break;
        case Page::SetStartPoint:
        break;
        case Page::SetEndPoint:
        break;
        case Page::SetDuration:
        break;
        case Page::Settings:
        break;
        case Page::Button_N_TextSize:
        break;
    }
}

void Menu::NewPage(Page page){
    //clear the ui array, and create a new one
    this->LastPage = this->CurrentPage;
    this->Cursor = 0;
    delete [] this->UI_Elements;
    switch(page){
        //Two elements: quickstart button and settings button
        case Page::MainMenu:
            ui_counts = 2;
            this->UI_Elements = new Element[3]{
                //adjust the coordinate later
                Button<std::function<void()>>(this->size, Vector2(10,20), "Quick Start", [this](){this->fn_SetPage(Page::QuickStart);}),
                Button<std::function<void()>>(this->size, Vector2(50,60), "Settings", [this](){fn_SetPage(Page::Settings);})
            };
        break;
        //Three elements
        case Page::QuickStart:
            ui_counts = 2;
            this->UI_Elements = new Element[4]{
                //adjust the coordinate later
                Text(this->size, Vector2(10,20), "Set Start Point"),
                Button<decltype(&fn_SetStartPoint)>(this->size, Vector2(50,60), "Set", fn_SetStartPoint),
                Button<decltype(&fn_Back)>(this->size, Vector2(70,80), "Back", fn_Back)
            };
        break;
        case Page::SetStartPoint:
            ui_counts = 2;
            this->UI_Elements = new Element[4]{
                //adjust the coordinate later
                Text(this->size, Vector2(10,20), "Set End Point"),
                Button<decltype(fn_SetEndPoint)>(this->size, Vector2(50,60), "Set", fn_SetEndPoint),
                Button<decltype(&fn_Back)>(this->size, Vector2(70,80), "Back", fn_Back)
            };
        break;
        //Three elements, but special case
        case Page::SetEndPoint:
            ui_counts = 3;
            this->UI_Elements = new Element[10]{
                //adjust the coordinate later
                Text(this->size, Vector2(10,20), "Set Duration"),
                OneNumCounter(this->size,Vector2(30,40)),
                OneNumCounter(this->size,Vector2(35,40)),
                OneNumCounter(this->size,Vector2(40,40)),
                OneNumCounter(this->size,Vector2(45,40)),
                //We'll draw a dot here
                OneNumCounter(this->size,Vector2(50,40)),
                OneNumCounter(this->size,Vector2(55,40)),
                Button<decltype(&fn_Back)>(this->size, Vector2(50,60), "Confirm",fn_SetDuration),
                Button<decltype(&fn_Back)>(this->size, Vector2(70,80), "Back", fn_Back)
            };
        break;
        case Page::SetDuration:
            ui_counts = 2;
            this->UI_Elements = new Element[6]{
                //adjust the coordinate later
                Text(this->size, Vector2(10,20), 
                "Start: "),
                Text(this->size,Vector2(20,20),
                "End:   "),
                Text(this->size,Vector2(30,30),
                "Time: "),
                Button<decltype(&fn_Back)>(this->size, Vector2(50,60), "Go",fn_Go),
                Button<decltype(&fn_Back)>(this->size, Vector2(70,80), "Back",fn_Back)
            };
        break;
        case Page::Settings:
            ui_counts = 2;
            this->UI_Elements = new Element[3]{
                //adjust the coordinate later
                Button<std::function<void()>>(this->size, Vector2(50,60), "Set Size",[this](){fn_SetPage(Page::Button_N_TextSize);}),
                Button<decltype(&fn_Back)>(this->size, Vector2(70,80), "Back",fn_Back)
            };
        break;
        //4 ui elements, but special case
        case Page::Button_N_TextSize:
            ui_counts = 4;
            this->UI_Elements = new Element[10]{
                //adjust the coordinate later
                Text(this->size, Vector2(10,20), "Set Size"),
                OneNumCounter(this->size,Vector2(30,40)),
                OneNumCounter(this->size,Vector2(35,40)),
                OneNumCounter(this->size,Vector2(40,40)),
                OneNumCounter(this->size,Vector2(45,40)),
                //We'll draw a dot here
                OneNumCounter(this->size,Vector2(50,40)),
                OneNumCounter(this->size,Vector2(55,40)),
                Button<decltype(&fn_Back)>(this->size, Vector2(50,60), "Set",fn_SetSize),
                Button<decltype(&fn_Back)>(this->size, Vector2(70,80), "Back",fn_Back)
            };
        break;
    }
}

void Menu::HandleInput(){
    switch (this->CurrentPage)
    {
        case Page::MainMenu:    case Page::QuickStart:  case Page::SetStartPoint:
        case Page::SetEndPoint: case Page::Settings:
            switch (this->input){
            case ButtonInput::Up :   case ButtonInput::Left :
                this->Cursor  = (Cursor + 1) % ui_counts; 
                break;
            case ButtonInput::Down : case ButtonInput::Right :
                this->Cursor  = (Cursor == 0) ? ui_counts -1 : Cursor - 1; 
                break;
            case ButtonInput::Ok:
                for (int i = 0; i < this->ui_counts; i++)
                {
                    if(UI_Elements[i].fn_GetCursorOn())
                        UI_Elements[i].OnClick();
                }
                break;
            }
            for(int i = 0;i < this->ui_counts; i++){
                if(i == Cursor){
                    UI_Elements[i].fn_SetCursorOn(true);
                    continue;
                }
                UI_Elements[i].fn_SetCursorOn(false);
            }
        case Page::SetDuration:
        
        break;
        case Page::Button_N_TextSize:
        break;
    }
}

template<typename T>
void Button<T>::OnClick(){
    if(OnClicked != nullptr)
        OnClicked();
}
template<typename T>
bool Button<T>::fn_GetCursorOn(){
    return this->CursorOn;
}
template<typename T>
void Button<T>::fn_SetCursorOn(bool a){
    this->CursorOn = a;
}


