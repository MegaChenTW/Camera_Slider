#pragma once
#include <string>
#include <functional>

enum ButtonInput{
    Up,
    Down,
    Right,
    Left,
    Ok,
    None
};
enum Page{
    MainMenu,
    QuickStart,
    SetStartPoint,
    SetEndPoint,
    SetDuration,
    Settings,
    Button_N_TextSize
};
struct Vector2{
    int x;
    int y;
    Vector2(int a, int b) : x(a),y(b){}
};

class Element{
    public:
    Element() ;
    virtual ~Element() {};
    virtual void OnClick();
    virtual bool fn_GetCursorOn();
    virtual void fn_SetCursorOn(bool);
};


class Menu{
    public:
        float *duaration;
        Vector2 *size;
        ButtonInput input;
        //try to render and store only the elements in the current page, so
        //dynamic arrays are used here 
        //UI elements in this page
        Element * UI_Elements;
        //the number of accessible ui element in this page
        int ui_counts = 0;

        Menu(float &Du,Vector2 &Size){
            input = ButtonInput::None;
        }
        void DrawMenu();
        //dynamically arrange the elements array so it uses less space
        void NewPage(Page page);
        void HandleInput();

        void fn_SetPage(Page page);
        void fn_Back();
        void fn_SetStartPoint();
        void fn_SetEndPoint();
        void fn_SetDuration();
        void fn_Go();
        void fn_SetSize();

    protected:
        Page CurrentPage;
        Page LastPage = Page::MainMenu;
        int Cursor;
};


class Text : public Element{
    public:
    Vector2 *Size,Coordinate;
    std::string DisplayText; 
    Text(Vector2 *v2,Vector2 Coo,std::string text) : Size(v2),Coordinate(Coo), DisplayText(text) {}
};

template<typename Func>

class Button : public Text {
    public:
    bool CursorOn ;
    typedef Func OnClickCallback;
    
    Button(Vector2 *v2,Vector2 Coo, std::string text,OnClickCallback func) : 
    Text(v2,Coo,text) , OnClicked(func)
    {  
        CursorOn = false;
    };
    void OnClick() override;
    bool fn_GetCursorOn() override;
    void fn_SetCursorOn(bool) override;
    protected:
    OnClickCallback OnClicked;
};

class OneNumCounter : public Element{
    public:
    Vector2 *Size,Coordinate;
    int count;
    OneNumCounter(Vector2 *size,Vector2 Coo) : Size(size),Coordinate(Coo){
        count = 0;
    }
    OneNumCounter(Vector2 *size,Vector2 Coo,int value): Size(size),Coordinate(Coo),count(value){}

    bool fn_GetCursorOn() override;
    void fn_SetCursorOn(bool) override;
};

