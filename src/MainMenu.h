#ifndef _MAIN_MENU_INCLUDE
#define _MAIN_MENU_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Sprite.h"

class MainMenu
{
public:
    MainMenu();
    ~MainMenu();

    void init();
    void update(int deltaTime);
    void render();

private:
    void initShaders();
    void initSpriteBackground();
    //void printTextsActions();

private:
    ShaderProgram texProgram;
    Texture spritesheetBackground;
    Sprite* background;
    glm::mat4 projection;
    float currentTime;
};

#endif // _MAIN_MENU_INCLUDE