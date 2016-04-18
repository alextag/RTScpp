
class Button
{
    public:
        Button( Texture*, Texture*, int, int );
        ~Button();
        Texture* get_texture();
        int x,y,sizex,sizey;
        void press();
        void release();
        bool isPressed;

    private:
        Texture *pressed,*notpressed;

};

Button::Button( Texture* pressed, Texture* notpressed, int x, int y) {
    this->isPressed = false;
    this->pressed = pressed;
    this->notpressed = notpressed;
    this->x = x;
    this->y = y;
    this->sizex = this->pressed->width;
    this->sizey = this->pressed->height;
}

Button::~Button() {
}

Texture* Button::get_texture() {
    if (this->isPressed) {
        return this->pressed;
    }
    return this->notpressed;
}

void Button::press() {
    this->isPressed = true;
return;}

void Button::release() {
    this->isPressed = false;
return;}
