#pragma once
#include <SFML/Graphics.hpp>
#include"mainLogic.h"

class SFMLApp
{
private:
    sf::RenderWindow window;
    MainLogic mainLogic;

    enum Screen { LOGIN, PATIENT_MENU, DOCTOR_MENU, ADMIN_MENU, OUTPUT_SCREEN };

    Screen currentScreen;
    Screen previousScreen;  // to know where to go back

    Patient* currentPatient;
    Doctor* currentDoctor;
    Admin* currentAdmin;

    sf::Font font;

    // INPUT BOX
    char inputBuffer[500];
    int inputLen;
    bool inputActive;

    // OUTPUT PANEL
    char outputLines[30][200];  // 30 lines, 200 chars each
    int outputCount;

    // STATE MACHINE
    int currentState;  // tracks which step of a function we're on
    int tempInt1;      // temporary storage between steps
    int tempInt2;
    float tempFloat1;
    char tempStr1[200];
    char tempStr2[200];
    char tempStr3[200];
    char tempStr4[200];

    int scrollOffset;


public:
    SFMLApp();
    void run();

private:
    // SCREEN DRAWING
    void drawLoginScreen();
    void drawPatientMenu();
    void drawDoctorMenu();
    void drawAdminMenu();

    // INPUT/OUTPUT HELPERS
    void drawInputBox(float x, float y, float w);
    void drawOutputPanel();
    void clearOutput();
    void addOutput(const char* line);
    void clearInput();
    void processEnter();  // called when Enter is pressed
    void drawOutputScreen();
    void goToOutputScreen(Screen prev);

    // BUTTON HELPERS
    sf::RectangleShape makeButton(float x, float y, float w, float h, sf::Color color);
    sf::Text makeText(const char* str, float x, float y, int size, sf::Color color);
    bool isClicked(sf::RectangleShape& btn, sf::Vector2f mousePos);
};