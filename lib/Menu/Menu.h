#ifndef _Menu
#define _Menu

#include <string>
#include <functional>
/// Declarations of random stuff /// 
namespace __Enum{
enum JoystickInput{
    Up,
    Down,
    Right,
    Left,
    Ok,
    None
};

enum Element_Type{
    none,
    Text,
    Button,
    NumPicker,
    Circle
};

}

struct Vector2{
    int x;
    int y;
    Vector2(){x = 0;y = 0;}
    Vector2(int a, int b) : x(a),y(b){}
};
/// Declaration of Elements and its child ///
class Element{
    public:
    Vector2 Size,Coordinate;
    Element(Vector2 const &v2,Vector2 Coo) : Size(v2), Coordinate(Coo){};
    Element(): Size(Vector2(10,10)), Coordinate(Vector2(10,10)){}
    virtual ~Element() {};
    virtual void OnClick(){};
    virtual int GetType() {return 0;}
    virtual const char* GetString(){}
    virtual void SetString(const char*){}
    virtual void SetString(const std::string&){};
    virtual bool fn_GetCursorOn(){return false;}
    virtual void fn_SetCursorOn(bool){}
    virtual int GetVal(){return 0;}
    virtual void SetVal(int){}
};

class Text : public Element{
    public:
    std::string DisplayText; 
    Text(Vector2 v2,Vector2 Coo, char* text) : Element(v2,Coo), DisplayText(text) {}
    int GetType() override {return 1;}
    void SetString(const char*);
    void SetString(const std::string&);
    const char* GetString() override { return this->DisplayText.c_str();}
};

class Button : public Element {
    public:
    bool CursorOn ;
    char* DisplayText;
    Button(Vector2 v2,Vector2 Coo, char* text, void func()) : 
    Element(v2, Coo), DisplayText(text) , OnClickFunc(func) , CursorOn(false) {};
    //Type: What's the parameter type of OnClick()
    int GetType() override {return 2;}
    char* GetString() override;
    void OnClick();
    bool fn_GetCursorOn() override;
    void fn_SetCursorOn(bool) override;

    protected:
    //Somehow, it works
    void (*OnClickFunc)();
};

class NumPicker : public Element{
    public:
    bool CursorOn;
    int count;
    NumPicker(Vector2 size,Vector2 Coo) : Element(size,Coo), count(0), CursorOn(false){}
    //NumPicker(Vector2 size,Vector2 Coo,int value): Element(size,Coo), count(value){}
    int GetType() override {return 3;}
    int GetVal() override {return count;}
    void SetVal(int a) override {count = a;}
    bool fn_GetCursorOn() override;
    void fn_SetCursorOn(bool) override;
};

namespace BasicShape{
    class Circle : public Element{
        public:
        Circle(Vector2 size,Vector2 Coo) : Element(size,Coo){};
        int GetType() override {return 4;}
    };
}

/// Declaration of Page ///
class Page{
    public:
    //Parameter: Page(int id,int element_couts,int interactive_count,Element figets[])
    Page(int a,int b,int c,Element **d):id(a),elements_count(b),interactive_count(c),figets(d){}
    Page(){}
    //Page();
    ~Page();
    int id;
    int elements_count;
    int interactive_count;
    Element **figets;
};



#endif