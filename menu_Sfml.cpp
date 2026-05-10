//#include <SFML/Graphics.hpp>
//#include <iostream>
//
//using namespace std;
//using namespace sf;
//
//void MakeText(Font& tempFont, RenderWindow& window) //passing by reference to avoid copies
//{
//    sf::Text mainHeading(tempFont, "Stat Of The Art Hospital", 22);//1st paramter font, 2nd the actual tex, 3rd font size
//
//    mainHeading.setPosition({ 120.f, 20.f }); //formatting the text
//    mainHeading.setFillColor(sf::Color::Cyan);
//    window.draw(mainHeading);// displays text made
//}
//
//Font LoadFont()
//{
//    Font fontForMenu;
//    fontForMenu.openFromFile("manrope-bold.otf");
//
//    return fontForMenu;
//}
//
//sf::RectangleShape MakeButtons(RenderWindow& window, Font& tempFont, float width, float height)
//{
//    sf::RectangleShape menuButtons({ 300.f,45.f });
//
//    menuButtons.setPosition({ width, height });
//
//    menuButtons.setFillColor(sf::Color::Magenta);
//
//    return menuButtons;
//}
//
//sf::Text CreateText(Font& tempFont, const char* buttonHeadings, float width, float height)
//{
//    sf::Text instructionLine(tempFont, buttonHeadings, 20);
//
//    instructionLine.setPosition({ width + 25, height + 10 });
//    instructionLine.setFillColor(sf::Color::White);
//
//    instructionLine.setOutlineThickness(2.f);
//    instructionLine.setOutlineColor(sf::Color::Black);
//
//    return instructionLine;
//}
//
//void UpdateButtonColors(sf::RectangleShape& patientButton, sf::RectangleShape& doctorButton, sf::RectangleShape& adminButton, 
//    sf::RectangleShape& exitButton, int currentOption)
//{
//    if (currentOption == 0)
//    {
//        patientButton.setFillColor(sf::Color::Blue);
//    }
//
//    else if (currentOption == 1)
//    {
//        doctorButton.setFillColor(sf::Color::Blue);
//    }
//
//    else if (currentOption == 2)
//    {
//        adminButton.setFillColor(sf::Color::Blue);
//    }
//
//    else if (currentOption == 3)
//    {
//        exitButton.setFillColor(sf::Color::Red);// exiting will give a different colour
//    }  
//}
//
//void DrawAllButtons(sf::RenderWindow& window, sf::RectangleShape& patientButton, sf::Text& patientText,
//    sf::RectangleShape& doctorButton, sf::Text& doctorText, sf::RectangleShape& adminButton, sf::Text& adminText,
//    sf::RectangleShape& exitButton, sf::Text& exitText) // this function will display everything
//{
//    window.draw(patientButton);
//    window.draw(patientText);
//
//    window.draw(doctorButton);
//    window.draw(doctorText);
//
//    window.draw(adminButton);
//    window.draw(adminText);
//
//    window.draw(exitButton);
//    window.draw(exitText);
//}
//
//void ResetColours(sf::RectangleShape& patientButton, sf::RectangleShape& doctorButton, sf::RectangleShape& adminButton,
//    sf::RectangleShape& exitButton, sf::Text& patientText, sf::Text& doctorText, sf::Text& adminText, 
//    sf::Text& exitText)// this functions will reset coulours
//{
//    patientButton.setFillColor(sf::Color::Cyan);
//    doctorButton.setFillColor(sf::Color::Cyan);
//    adminButton.setFillColor(sf::Color::Cyan);
//    exitButton.setFillColor(sf::Color::Cyan);
//
//    patientText.setFillColor(sf::Color::White);
//    doctorText.setFillColor(sf::Color::White);
//    adminText.setFillColor(sf::Color::White);
//    exitText.setFillColor(sf::Color::White);
//}
//
//
//int main()
//{
//    int currentOption = 0;// initially user is at the first option
//    const int totalNumberOfOptions = 4;
//
//    unsigned int windowHorizontalDimension = 1000.f;
//    unsigned int  windowVerticalDimension = 400.f;
//
//    const char* patientChar = "1. Patient";
//    const char* doctorChar = "2. Doctor";
//    const char* adminChar = "3. Admin";
//    const char* exitChar = "4.  Exit";
//
//    sf::RenderWindow window(sf::VideoMode({ windowHorizontalDimension,windowVerticalDimension }), "Welcome to Medicore");
//    Font mainFont= LoadFont();// this will load the font present in the folder
//
//    float centralXDimension = (windowHorizontalDimension - 300.f) / 2.f;
//
//    sf::RectangleShape PatientButton = MakeButtons(window, mainFont, centralXDimension, 150.f);
//    sf::RectangleShape DoctorButton = MakeButtons(window, mainFont, centralXDimension, 220.f);
//    sf::RectangleShape AdminButton = MakeButtons(window, mainFont, centralXDimension, 290.f);
//    sf::RectangleShape ExitButton = MakeButtons(window, mainFont, centralXDimension, 360.f);
//
//    sf::Text PatientText = CreateText(mainFont, patientChar, centralXDimension, 150.f);
//    sf::Text DoctorText = CreateText(mainFont, doctorChar, centralXDimension, 220.f);
//    sf::Text AdminText = CreateText(mainFont, adminChar, centralXDimension, 290.f);
//    sf::Text ExitText = CreateText(mainFont, exitChar, centralXDimension, 360.f);
//
//
//    while (window.isOpen())
//    {
//        while (std::optional event = window.pollEvent())
//        {
//            if (event->is<sf::Event::Closed>())
//            {
//                window.close();
//            }
//
//            if (auto* keyboardKey = event->getIf<sf::Event::KeyPressed>())
//            {
//                if (keyboardKey->code == sf::Keyboard::Key::Up)
//                {
//                    currentOption--;
//
//                    if (currentOption < 0)
//                    {
//                        currentOption = totalNumberOfOptions - 1;
//                    }
//                }
//
//                if (keyboardKey->code == sf::Keyboard::Key::Down)
//                {
//                    currentOption++;
//
//                    if (currentOption > 3)// if already at last option, user will come at first option
//                    {
//                        currentOption = 0;
//                    }
//                }
//
//                if (keyboardKey->code == sf::Keyboard::Key::Enter)
//                {
//                    if (currentOption == 3)
//                    {
//                        window.close();
//                    }
//                    else
//                    {
//                        cout << "The option you have chosen is " << currentOption + 1 << endl;
//                    }
//                }
//            }
//        }
//
//        window.clear();
//
//        MakeText(mainFont, window); //making text
//
//        ResetColours(PatientButton, DoctorButton, AdminButton, ExitButton, PatientText, DoctorText, AdminText, ExitText);
//        UpdateButtonColors(PatientButton, DoctorButton, AdminButton, ExitButton, currentOption);
//
//        DrawAllButtons(window, PatientButton, PatientText, DoctorButton, DoctorText, AdminButton, AdminText, ExitButton,
//            ExitText);
//
//        window.display();// showing the window frame by frame
//    }
//    return 0;
//}