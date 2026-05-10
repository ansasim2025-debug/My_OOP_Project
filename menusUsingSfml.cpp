#define _CRT_SECURE_NO_WARNINGS

#include "menusUsingSfml.h"
#include <iostream>
using namespace std;

SFMLApp::SFMLApp()
    : window(sf::VideoMode({ 1024, 768 }), "MediCore Hospital Management System"),
    currentScreen(LOGIN),
    previousScreen(LOGIN),
    currentPatient(nullptr),
    currentDoctor(nullptr),
    currentAdmin(nullptr),
    inputLen(0),
    inputActive(true),
    outputCount(0),
    scrollOffset(0),
    currentState(0),
    tempInt1(0),
    tempInt2(0),
    tempFloat1(0.0f)
{
    if (!font.openFromFile("manrope-bold.otf"))
    {
        cout << "Failed to load font!" << endl;
    }
    window.setFramerateLimit(60);

    inputBuffer[0] = '\0';
    tempStr1[0] = '\0';
    tempStr2[0] = '\0';
    tempStr3[0] = '\0';
    tempStr4[0] = '\0';

    for (int i = 0; i < 30; i++)
        outputLines[i][0] = '\0';
}

void SFMLApp::clearOutput()
{
    outputCount = 0;
    scrollOffset = 0;
    for (int i = 0; i < 30; i++)
        outputLines[i][0] = '\0';
}

void SFMLApp::addOutput(const char* line)
{
    if (outputCount >= 30) return;

    int i = 0;
    while (line[i] != '\0' && i < 199)
    {
        outputLines[outputCount][i] = line[i];
        i++;
    }
    outputLines[outputCount][i] = '\0';
    outputCount++;
}

void SFMLApp::clearInput()
{
    inputBuffer[0] = '\0';
    inputLen = 0;
}

void SFMLApp::drawInputBox(float x, float y, float w)
{
    sf::RectangleShape box(sf::Vector2f(w, 40));
    box.setPosition(sf::Vector2f(x, y));
    box.setFillColor(sf::Color(50, 50, 50));
    box.setOutlineColor(sf::Color(70, 130, 180));
    box.setOutlineThickness(2);
    window.draw(box);

    sf::Text inputText = makeText(inputBuffer, x + 8, y + 8, 18, sf::Color::White);
    window.draw(inputText);
}

void SFMLApp::drawOutputScreen()
{
    // TITLE BAR
    sf::RectangleShape titleBar(sf::Vector2f(1024, 60));
    titleBar.setPosition(sf::Vector2f(0, 0));
    titleBar.setFillColor(sf::Color(40, 40, 40));
    window.draw(titleBar);

    sf::Text title = makeText("MediCore - Output", 350, 15, 28, sf::Color::White);
    window.draw(title);

    // BACK BUTTON
    sf::RectangleShape backBtn = makeButton(20, 10, 100, 40, sf::Color(180, 70, 70));
    sf::Text backLbl = makeText("Back", 45, 18, 18, sf::Color::White);
    window.draw(backBtn);
    window.draw(backLbl);

    // SCROLL UP BUTTON
    sf::RectangleShape upBtn = makeButton(950, 80, 60, 35, sf::Color(70, 100, 130));
    sf::Text upLbl = makeText("UP", 963, 88, 16, sf::Color::White);
    window.draw(upBtn);
    window.draw(upLbl);

    // SCROLL DOWN BUTTON
    sf::RectangleShape downBtn = makeButton(950, 125, 60, 35, sf::Color(70, 100, 130));
    sf::Text downLbl = makeText("DOWN", 953, 133, 14, sf::Color::White);
    window.draw(downBtn);
    window.draw(downLbl);

    // OUTPUT LINES - show 25 lines at a time
    int maxVisibleLines = 25;
    for (int i = scrollOffset; i < outputCount && i < scrollOffset + maxVisibleLines; i++)
    {
        sf::Text line = makeText(outputLines[i], 30, 80 + ((i - scrollOffset) * 24), 17, sf::Color(200, 200, 200));
        window.draw(line);
    }

    // SCROLL INDICATOR
    if (outputCount > maxVisibleLines)
    {
        // build "showing X-Y of Z"
        char info[50];
        int pos = 0;

        const char* pre = "Lines: ";
        for (int k = 0; pre[k]; k++) info[pos++] = pre[k];

        // current start
        int start = scrollOffset + 1;
        char tmp[10]; int len = 0; int v = start;
        if (v == 0) { info[pos++] = '0'; }
        else { while (v > 0) { tmp[len++] = '0' + v % 10; v /= 10; } for (int x = len - 1; x >= 0; x--) info[pos++] = tmp[x]; }

        info[pos++] = '-';

        // current end
        int end = scrollOffset + maxVisibleLines;
        if (end > outputCount) end = outputCount;
        len = 0; v = end;
        if (v == 0) { info[pos++] = '0'; }
        else { while (v > 0) { tmp[len++] = '0' + v % 10; v /= 10; } for (int x = len - 1; x >= 0; x--) info[pos++] = tmp[x]; }

        const char* of = " of ";
        for (int k = 0; of[k]; k++) info[pos++] = of[k];

        len = 0; v = outputCount;
        if (v == 0) { info[pos++] = '0'; }
        else { while (v > 0) { tmp[len++] = '0' + v % 10; v /= 10; } for (int x = len - 1; x >= 0; x--) info[pos++] = tmp[x]; }

        info[pos] = '\0';
        sf::Text scrollText = makeText(info, 750, 740, 14, sf::Color(150, 150, 150));
        window.draw(scrollText);
    }

    // INPUT BOX if needed
    if (currentState != 0)
    {
        sf::RectangleShape inputBg(sf::Vector2f(1024, 60));
        inputBg.setPosition(sf::Vector2f(0, 700));
        inputBg.setFillColor(sf::Color(40, 40, 40));
        window.draw(inputBg);

        sf::Text prompt = makeText(">", 15, 715, 20, sf::Color(70, 130, 180));
        window.draw(prompt);
        drawInputBox(35, 710, 950);
    }
}

sf::RectangleShape SFMLApp::makeButton(float x, float y, float w, float h, sf::Color color)
{
    sf::RectangleShape btn(sf::Vector2f(w, h));
    btn.setPosition(sf::Vector2f(x, y));
    btn.setFillColor(color);
    return btn;
}

sf::Text SFMLApp::makeText(const char* str, float x, float y, int size, sf::Color color)
{
    sf::Text text(font, str, size);
    text.setPosition(sf::Vector2f(x, y));
    text.setFillColor(color);
    return text;
}

bool SFMLApp::isClicked(sf::RectangleShape& btn, sf::Vector2f mousePos)
{
    return btn.getGlobalBounds().contains(mousePos);
}

// HELPER - switch to output screen
void SFMLApp::goToOutputScreen(Screen prev)
{
    previousScreen = prev;
    currentScreen = OUTPUT_SCREEN;
    scrollOffset = 0;
}

void SFMLApp::run()
{
    sf::View view(sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(1024, 768)));
    window.setView(view);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            // KEYBOARD INPUT
            if (const auto* keyPressed = event->getIf<sf::Event::TextEntered>())
            {
                if (keyPressed->unicode == 8 && inputLen > 0)
                {
                    inputLen--;
                    inputBuffer[inputLen] = '\0';
                }
                else if (keyPressed->unicode == 13)
                {
                    processEnter();
                }
                else if (keyPressed->unicode >= 32 && keyPressed->unicode < 127 && inputLen < 499)
                {
                    inputBuffer[inputLen] = (char)keyPressed->unicode;
                    inputLen++;
                    inputBuffer[inputLen] = '\0';
                }
            }

            // MOUSE WHEEL SCROLLING
            if (const auto* mouseWheel = event->getIf<sf::Event::MouseWheelScrolled>())
            {
                if (currentScreen == OUTPUT_SCREEN)
                {
                    if (mouseWheel->delta > 0 && scrollOffset > 0)
                        scrollOffset--;
                    else if (mouseWheel->delta < 0 && scrollOffset < outputCount - 1)
                        scrollOffset++;
                }
            }

            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mousePressed->button == sf::Mouse::Button::Left)
                {
                    sf::Vector2f mousePos = window.mapPixelToCoords(
                        sf::Vector2i(mousePressed->position.x, mousePressed->position.y), view);

                    // OUTPUT SCREEN BUTTONS
                    if (currentScreen == OUTPUT_SCREEN)
                    {
                        sf::RectangleShape backBtn = makeButton(20, 10, 100, 40, sf::Color(180, 70, 70));
                        sf::RectangleShape upBtn = makeButton(950, 80, 60, 35, sf::Color(70, 100, 130));
                        sf::RectangleShape downBtn = makeButton(950, 125, 60, 35, sf::Color(70, 100, 130));

                        if (isClicked(backBtn, mousePos))
                        {
                            currentScreen = previousScreen;
                            clearOutput();
                            currentState = 0;
                        }
                        else if (isClicked(upBtn, mousePos))
                        {
                            if (scrollOffset > 0) scrollOffset--;
                        }
                        else if (isClicked(downBtn, mousePos))
                        {
                            if (scrollOffset < outputCount - 1) scrollOffset++;
                        }
                    }

                    else if (currentScreen == LOGIN)
                    {
                        sf::RectangleShape btnPatient = makeButton(387, 300, 250, 50, sf::Color(70, 130, 180));
                        sf::RectangleShape btnDoctor = makeButton(387, 370, 250, 50, sf::Color(70, 130, 180));
                        sf::RectangleShape btnAdmin = makeButton(387, 440, 250, 50, sf::Color(70, 130, 180));
                        sf::RectangleShape btnExit = makeButton(387, 510, 250, 50, sf::Color(180, 70, 70));

                        if (isClicked(btnPatient, mousePos))
                        {
                            clearOutput();
                            addOutput("*** Patient Login ***");
                            addOutput("Enter ID:");
                            currentState = 100;
                            goToOutputScreen(LOGIN);
                        }
                        else if (isClicked(btnDoctor, mousePos))
                        {
                            clearOutput();
                            addOutput("*** Doctor Login ***");
                            addOutput("Enter ID:");
                            currentState = 102;
                            goToOutputScreen(LOGIN);
                        }
                        else if (isClicked(btnAdmin, mousePos))
                        {
                            clearOutput();
                            addOutput("*** Admin Login ***");
                            addOutput("Enter ID:");
                            currentState = 104;
                            goToOutputScreen(LOGIN);
                        }
                        else if (isClicked(btnExit, mousePos))
                        {
                            window.close();
                        }
                    }

                    else if (currentScreen == PATIENT_MENU)
                    {
                        sf::RectangleShape btn1 = makeButton(387, 200, 250, 45, sf::Color(70, 130, 180));
                        sf::RectangleShape btn2 = makeButton(387, 255, 250, 45, sf::Color(70, 130, 180));
                        sf::RectangleShape btn3 = makeButton(387, 310, 250, 45, sf::Color(70, 130, 180));
                        sf::RectangleShape btn4 = makeButton(387, 365, 250, 45, sf::Color(70, 130, 180));
                        sf::RectangleShape btn5 = makeButton(387, 420, 250, 45, sf::Color(70, 130, 180));
                        sf::RectangleShape btn6 = makeButton(387, 475, 250, 45, sf::Color(70, 130, 180));
                        sf::RectangleShape btn7 = makeButton(387, 530, 250, 45, sf::Color(70, 130, 180));
                        sf::RectangleShape btn8 = makeButton(387, 585, 250, 45, sf::Color(180, 70, 70));

                        if (isClicked(btn1, mousePos))
                        {
                            clearOutput();
                            addOutput("Enter specialization to search:");
                            currentState = 200;
                            goToOutputScreen(PATIENT_MENU);
                        }
                        else if (isClicked(btn2, mousePos))
                        {
                            clearOutput();
                            mainLogic.displayPendingAppointmentsSFML(*currentPatient, outputLines, outputCount);
                            addOutput("Enter Appointment ID to cancel:");
                            currentState = 204;
                            goToOutputScreen(PATIENT_MENU);
                        }
                        else if (isClicked(btn3, mousePos))
                        {
                            clearOutput();
                            mainLogic.viewAppointmentsSFML(*currentPatient, outputLines, outputCount);
                            currentState = 0;
                            goToOutputScreen(PATIENT_MENU);
                        }
                        else if (isClicked(btn4, mousePos))
                        {
                            clearOutput();
                            mainLogic.viewMedicalRecordsSFML(*currentPatient, outputLines, outputCount);
                            currentState = 0;
                            goToOutputScreen(PATIENT_MENU);
                        }
                        else if (isClicked(btn5, mousePos))
                        {
                            clearOutput();
                            mainLogic.viewBillsSFML(*currentPatient, outputLines, outputCount);
                            currentState = 0;
                            goToOutputScreen(PATIENT_MENU);
                        }
                        else if (isClicked(btn6, mousePos))
                        {
                            clearOutput();
                            mainLogic.displayUnpaidBillsSFML(*currentPatient, outputLines, outputCount);
                            addOutput("Enter Bill ID to pay:");
                            currentState = 205;
                            goToOutputScreen(PATIENT_MENU);
                        }
                        else if (isClicked(btn7, mousePos))
                        {
                            clearOutput();
                            addOutput("Enter amount to add (PKR):");
                            currentState = 206;
                            goToOutputScreen(PATIENT_MENU);
                        }
                        else if (isClicked(btn8, mousePos))
                        {
                            currentPatient = nullptr;
                            currentScreen = LOGIN;
                            currentState = 0;
                            clearOutput();
                        }
                    }

                    else if (currentScreen == DOCTOR_MENU)
                    {
                        sf::RectangleShape btn1 = makeButton(387, 250, 250, 45, sf::Color(70, 130, 180));
                        sf::RectangleShape btn2 = makeButton(387, 305, 250, 45, sf::Color(70, 130, 180));
                        sf::RectangleShape btn3 = makeButton(387, 360, 250, 45, sf::Color(70, 130, 180));
                        sf::RectangleShape btn4 = makeButton(387, 415, 250, 45, sf::Color(70, 130, 180));
                        sf::RectangleShape btn5 = makeButton(387, 470, 250, 45, sf::Color(70, 130, 180));
                        sf::RectangleShape btn6 = makeButton(387, 525, 250, 45, sf::Color(180, 70, 70));

                        if (isClicked(btn1, mousePos))
                        {
                            clearOutput();
                            mainLogic.viewTodaysAppointmentsSFML(*currentDoctor, outputLines, outputCount);
                            currentState = 0;
                            goToOutputScreen(DOCTOR_MENU);
                        }
                        else if (isClicked(btn2, mousePos))
                        {
                            clearOutput();
                            mainLogic.displayPendingTodaySFML(*currentDoctor, outputLines, outputCount);
                            addOutput("Enter Appointment ID to mark complete:");
                            currentState = 300;
                            goToOutputScreen(DOCTOR_MENU);
                        }
                        else if (isClicked(btn3, mousePos))
                        {
                            clearOutput();
                            mainLogic.displayPendingTodaySFML(*currentDoctor, outputLines, outputCount);
                            addOutput("Enter Appointment ID to mark no-show:");
                            currentState = 301;
                            goToOutputScreen(DOCTOR_MENU);
                        }
                        else if (isClicked(btn4, mousePos))
                        {
                            clearOutput();
                            addOutput("Enter Appointment ID:");
                            currentState = 302;
                            goToOutputScreen(DOCTOR_MENU);
                        }
                        else if (isClicked(btn5, mousePos))
                        {
                            clearOutput();
                            addOutput("Enter Patient ID:");
                            currentState = 303;
                            goToOutputScreen(DOCTOR_MENU);
                        }
                        else if (isClicked(btn6, mousePos))
                        {
                            currentDoctor = nullptr;
                            currentScreen = LOGIN;
                            currentState = 0;
                            clearOutput();
                        }
                    }

                    else if (currentScreen == ADMIN_MENU)
                    {
                        sf::RectangleShape btn1 = makeButton(387, 150, 250, 40, sf::Color(70, 130, 180));
                        sf::RectangleShape btn2 = makeButton(387, 200, 250, 40, sf::Color(70, 130, 180));
                        sf::RectangleShape btn3 = makeButton(387, 250, 250, 40, sf::Color(70, 130, 180));
                        sf::RectangleShape btn4 = makeButton(387, 300, 250, 40, sf::Color(70, 130, 180));
                        sf::RectangleShape btn5 = makeButton(387, 350, 250, 40, sf::Color(70, 130, 180));
                        sf::RectangleShape btn6 = makeButton(387, 400, 250, 40, sf::Color(70, 130, 180));
                        sf::RectangleShape btn7 = makeButton(387, 450, 250, 40, sf::Color(70, 130, 180));
                        sf::RectangleShape btn8 = makeButton(387, 500, 250, 40, sf::Color(70, 130, 180));
                        sf::RectangleShape btn9 = makeButton(387, 550, 250, 40, sf::Color(70, 130, 180));
                        sf::RectangleShape btn10 = makeButton(387, 600, 250, 40, sf::Color(180, 70, 70));

                        if (isClicked(btn1, mousePos))
                        {
                            clearOutput();
                            addOutput("Enter doctor name:");
                            currentState = 400;
                            goToOutputScreen(ADMIN_MENU);
                        }
                        else if (isClicked(btn2, mousePos))
                        {
                            clearOutput();
                            mainLogic.viewAllDoctorsSFML(outputLines, outputCount);
                            addOutput("Enter Doctor ID to remove:");
                            currentState = 401;
                            goToOutputScreen(ADMIN_MENU);
                        }
                        else if (isClicked(btn3, mousePos))
                        {
                            clearOutput();
                            mainLogic.viewAllPatientsSFML(outputLines, outputCount);
                            currentState = 0;
                            goToOutputScreen(ADMIN_MENU);
                        }
                        else if (isClicked(btn4, mousePos))
                        {
                            clearOutput();
                            mainLogic.viewAllDoctorsSFML(outputLines, outputCount);
                            currentState = 0;
                            goToOutputScreen(ADMIN_MENU);
                        }
                        else if (isClicked(btn5, mousePos))
                        {
                            clearOutput();
                            mainLogic.viewAllAppointmentsSFML(outputLines, outputCount);
                            currentState = 0;
                            goToOutputScreen(ADMIN_MENU);
                        }
                        else if (isClicked(btn6, mousePos))
                        {
                            clearOutput();
                            mainLogic.viewUnpaidBillsSFML(outputLines, outputCount);
                            currentState = 0;
                            goToOutputScreen(ADMIN_MENU);
                        }
                        else if (isClicked(btn7, mousePos))
                        {
                            clearOutput();
                            addOutput("Enter Patient ID to discharge:");
                            currentState = 402;
                            goToOutputScreen(ADMIN_MENU);
                        }
                        else if (isClicked(btn8, mousePos))
                        {
                            clearOutput();
                            mainLogic.viewSecurityLogSFML(outputLines, outputCount);
                            currentState = 0;
                            goToOutputScreen(ADMIN_MENU);
                        }
                        else if (isClicked(btn9, mousePos))
                        {
                            clearOutput();
                            mainLogic.generateDailyReportSFML(outputLines, outputCount);
                            currentState = 0;
                            goToOutputScreen(ADMIN_MENU);
                        }
                        else if (isClicked(btn10, mousePos))
                        {
                            currentAdmin = nullptr;
                            currentScreen = LOGIN;
                            currentState = 0;
                            clearOutput();
                        }
                    }
                }
            }
        }

        window.clear(sf::Color(30, 30, 30));

        if (currentScreen == LOGIN)
            drawLoginScreen();
        else if (currentScreen == PATIENT_MENU)
            drawPatientMenu();
        else if (currentScreen == DOCTOR_MENU)
            drawDoctorMenu();
        else if (currentScreen == ADMIN_MENU)
            drawAdminMenu();
        else if (currentScreen == OUTPUT_SCREEN)
            drawOutputScreen();

        window.display();
    }
}

void SFMLApp::processEnter()
{
    // ==================== LOGIN STATES ====================

    if (currentState == 100)
    {
        tempInt1 = 0;
        int i = 0;
        while (inputBuffer[i] != '\0')
        {
            if (inputBuffer[i] < '0' || inputBuffer[i] > '9')
            {
                addOutput("Invalid ID. Numbers only.");
                clearInput();
                return;
            }
            tempInt1 = tempInt1 * 10 + (inputBuffer[i] - '0');
            i++;
        }
        clearInput();
        addOutput("Enter Password:");
        currentState = 101;
        return;
    }

    if (currentState == 101)
    {
        Patient* p = mainLogic.getPatients().findByID(tempInt1);
        if (p != nullptr && p->strCompare(p->getPassword(), inputBuffer))
        {
            clearInput();
            tempInt2 = 0;
            clearOutput();
            currentPatient = p;
            currentScreen = PATIENT_MENU;
            currentState = 0;
        }
        else
        {
            tempInt2++;
            clearInput();

            if (tempInt2 >= 3)
            {
                time_t t = time(0);
                char timestamp[50];
                strftime(timestamp, 50, "%d-%m-%Y %H:%M:%S", localtime(&t));
                mainLogic.getFileHandler().logSecurity(timestamp, "Patient", tempInt1, "FAILED");
                addOutput("Account locked. Contact admin.");
                currentState = 0;
                tempInt2 = 0;
            }
            else
            {
                char msg[100] = "Invalid. Attempts remaining: ";
                int len = 0;
                while (msg[len]) len++;
                msg[len] = '0' + (3 - tempInt2);
                msg[len + 1] = '\0';
                addOutput(msg);
                addOutput("Enter ID:");
                currentState = 100;
            }
        }
        return;
    }

    if (currentState == 102)
    {
        tempInt1 = 0;
        int i = 0;
        while (inputBuffer[i] != '\0')
        {
            if (inputBuffer[i] < '0' || inputBuffer[i] > '9')
            {
                addOutput("Invalid ID. Numbers only.");
                clearInput();
                return;
            }
            tempInt1 = tempInt1 * 10 + (inputBuffer[i] - '0');
            i++;
        }
        clearInput();
        addOutput("Enter Password:");
        currentState = 103;
        return;
    }

    if (currentState == 103)
    {
        Doctor* d = mainLogic.getDoctors().findByID(tempInt1);
        if (d != nullptr && d->strCompare(d->getPassword(), inputBuffer))
        {
            clearInput();
            tempInt2 = 0;
            clearOutput();
            currentDoctor = d;
            currentScreen = DOCTOR_MENU;
            currentState = 0;
        }
        else
        {
            tempInt2++;
            clearInput();

            if (tempInt2 >= 3)
            {
                time_t t = time(0);
                char timestamp[50];
                strftime(timestamp, 50, "%d-%m-%Y %H:%M:%S", localtime(&t));
                mainLogic.getFileHandler().logSecurity(timestamp, "Doctor", tempInt1, "FAILED");
                addOutput("Account locked. Contact admin.");
                currentState = 0;
                tempInt2 = 0;
            }
            else
            {
                char msg[100] = "Invalid. Attempts remaining: ";
                int len = 0;
                while (msg[len]) len++;
                msg[len] = '0' + (3 - tempInt2);
                msg[len + 1] = '\0';
                addOutput(msg);
                addOutput("Enter ID:");
                currentState = 102;
            }
        }
        return;
    }

    if (currentState == 104)
    {
        tempInt1 = 0;
        int i = 0;
        while (inputBuffer[i] != '\0')
        {
            if (inputBuffer[i] < '0' || inputBuffer[i] > '9')
            {
                addOutput("Invalid ID. Numbers only.");
                clearInput();
                return;
            }
            tempInt1 = tempInt1 * 10 + (inputBuffer[i] - '0');
            i++;
        }
        clearInput();
        addOutput("Enter Password:");
        currentState = 105;
        return;
    }

    if (currentState == 105)
    {
        Admin* a = mainLogic.getAdmins().findByID(tempInt1);
        if (a != nullptr && a->strCompare(a->getPassword(), inputBuffer))
        {
            clearInput();
            tempInt2 = 0;
            clearOutput();
            currentAdmin = a;
            currentScreen = ADMIN_MENU;
            currentState = 0;
        }
        else
        {
            tempInt2++;
            clearInput();

            if (tempInt2 >= 3)
            {
                time_t t = time(0);
                char timestamp[50];
                strftime(timestamp, 50, "%d-%m-%Y %H:%M:%S", localtime(&t));
                mainLogic.getFileHandler().logSecurity(timestamp, "Admin", tempInt1, "FAILED");
                addOutput("Account locked. Contact admin.");
                currentState = 0;
                tempInt2 = 0;
            }
            else
            {
                char msg[100] = "Invalid. Attempts remaining: ";
                int len = 0;
                while (msg[len]) len++;
                msg[len] = '0' + (3 - tempInt2);
                msg[len + 1] = '\0';
                addOutput(msg);
                addOutput("Enter ID:");
                currentState = 104;
            }
        }
        return;
    }

    // ==================== PATIENT STATES ====================

    if (currentState == 200)
    {
        clearOutput();
        int i = 0;
        while (inputBuffer[i]) { tempStr1[i] = inputBuffer[i]; i++; }
        tempStr1[i] = '\0';
        clearInput();

        bool found = false;
        Doctor* allDoctors = mainLogic.getDoctors().getAll();

        for (int d = 0; d < mainLogic.getDoctors().size(); d++)
        {
            char* docSpec = allDoctors[d].getSpecialization();

            int len1 = 0, len2 = 0;
            while (tempStr1[len1]) len1++;
            while (docSpec[len2]) len2++;

            bool match = (len1 == len2);
            int k = 0;
            while (match && tempStr1[k])
            {
                char c1 = tempStr1[k];
                char c2 = docSpec[k];
                if (c1 >= 'A' && c1 <= 'Z') c1 += 32;
                if (c2 >= 'A' && c2 <= 'Z') c2 += 32;
                if (c1 != c2) match = false;
                k++;
            }

            if (match)
            {
                char line[200];
                int li = 0;

                const char* pre = "ID: ";
                for (int x = 0; pre[x]; x++) line[li++] = pre[x];

                int id = allDoctors[d].getID();
                char idStr[10]; int idLen = 0;
                if (id == 0) { idStr[idLen++] = '0'; }
                else { char tmp[10]; int t = 0; while (id > 0) { tmp[t++] = '0' + id % 10; id /= 10; } for (int x = t - 1; x >= 0; x--) idStr[idLen++] = tmp[x]; }
                for (int x = 0; x < idLen; x++) line[li++] = idStr[x];

                const char* mid = " | ";
                for (int x = 0; mid[x]; x++) line[li++] = mid[x];
                const char* name = allDoctors[d].getName();
                for (int x = 0; name[x]; x++) line[li++] = name[x];
                for (int x = 0; mid[x]; x++) line[li++] = mid[x];
                const char* feePre = "Fee: PKR ";
                for (int x = 0; feePre[x]; x++) line[li++] = feePre[x];

                int fee = (int)allDoctors[d].getFee();
                char feeStr[10]; int feeLen = 0;
                if (fee == 0) { feeStr[feeLen++] = '0'; }
                else { char tmp[10]; int t = 0; while (fee > 0) { tmp[t++] = '0' + fee % 10; fee /= 10; } for (int x = t - 1; x >= 0; x--) feeStr[feeLen++] = tmp[x]; }
                for (int x = 0; x < feeLen; x++) line[li++] = feeStr[x];

                line[li] = '\0';
                addOutput(line);
                found = true;
            }
        }

        if (!found)
        {
            addOutput("No doctors available for that specialization.");
            currentState = 0;
        }
        else
        {
            addOutput("Enter Doctor ID:");
            currentState = 201;
        }
        return;
    }

    if (currentState == 201)
    {
        tempInt1 = 0;
        int i = 0;
        while (inputBuffer[i])
        {
            if (inputBuffer[i] < '0' || inputBuffer[i] > '9')
            {
                addOutput("Invalid ID. Numbers only.");
                clearInput();
                return;
            }
            tempInt1 = tempInt1 * 10 + (inputBuffer[i] - '0');
            i++;
        }
        clearInput();

        Doctor* d = mainLogic.getDoctors().findByID(tempInt1);
        if (d == nullptr)
        {
            addOutput("Doctor not found.");
            currentState = 0;
        }
        else
        {
            addOutput("Enter date (DD-MM-YYYY):");
            currentState = 202;
        }
        return;
    }

    if (currentState == 202)
    {
        if (!Validator::dateValidity(inputBuffer))
        {
            addOutput("Invalid date. Use format DD-MM-YYYY.");
            clearInput();
            return;
        }

        int i = 0;
        while (inputBuffer[i]) { tempStr2[i] = inputBuffer[i]; i++; }
        tempStr2[i] = '\0';
        clearInput();

        const char* allSlots[8] = { "09:00","10:00","11:00","12:00","13:00","14:00","15:00","16:00" };
        addOutput("Available slots:");

        Appointment* allAppts = mainLogic.getAppointments().getAll();
        for (int s = 0; s < 8; s++)
        {
            bool taken = false;
            for (int j = 0; j < mainLogic.getAppointments().size(); j++)
            {
               
                if (allAppts[j].getDoctorID() == tempInt1 &&
                    allAppts[j].strCompare(allAppts[j].getDate(), tempStr2) &&
                    allAppts[j].strCompare(allAppts[j].getTimeSlot(), allSlots[s]) &&
                    !allAppts[j].strCompare(allAppts[j].getStatus(), "cancelled"))
                {
                    taken = true;
                    break;
                }
            }
            if (!taken) addOutput(allSlots[s]);
        }

        addOutput("Enter time slot (e.g. 09:00):");
        currentState = 203;
        return;
    }

    if (currentState == 203)
    {
        if (!Validator::timeSlotValidity(inputBuffer))
        {
            addOutput("Invalid time slot.");
            clearInput();
            return;
        }

        int i = 0;
        while (inputBuffer[i]) { tempStr3[i] = inputBuffer[i]; i++; }
        tempStr3[i] = '\0';
        clearInput();

        Appointment* allAppts = mainLogic.getAppointments().getAll();
        bool taken = false;
        for (int j = 0; j < mainLogic.getAppointments().size(); j++)
        {
            if (allAppts[j].getDoctorID() == tempInt1 &&
                allAppts[j].strCompare(allAppts[j].getDate(), tempStr2) &&
                allAppts[j].strCompare(allAppts[j].getTimeSlot(), tempStr3) &&
                !allAppts[j].strCompare(allAppts[j].getStatus(), "cancelled"))
            {
                taken = true;
                break;
            }
        }

        if (taken)
        {
            addOutput("Slot already taken. Enter another time slot:");
            return;
        }

        Doctor* doc = mainLogic.getDoctors().findByID(tempInt1);
        if (currentPatient->getBalance() < doc->getFee())
        {
            addOutput("Insufficient funds. Please top up your balance.");
            currentState = 0;
            return;
        }

        mainLogic.bookAppointmentSFML(*currentPatient, tempInt1, tempStr2, tempStr3);
        addOutput("Appointment booked successfully!");
        currentState = 0;
        return;
    }

    if (currentState == 204)
    {
        tempInt1 = 0;
        int i = 0;
        while (inputBuffer[i])
        {
            if (inputBuffer[i] < '0' || inputBuffer[i] > '9')
            {
                addOutput("Invalid ID. Numbers only.");
                clearInput();
                return;
            }
            tempInt1 = tempInt1 * 10 + (inputBuffer[i] - '0');
            i++;
        }
        clearInput();
        mainLogic.cancelAppointmentSFML(*currentPatient, tempInt1);
        addOutput("Appointment cancelled successfully.");
        currentState = 0;
        return;
    }

    if (currentState == 205)
    {
        tempInt1 = 0;
        int i = 0;
        while (inputBuffer[i])
        {
            if (inputBuffer[i] < '0' || inputBuffer[i] > '9')
            {
                addOutput("Invalid ID. Numbers only.");
                clearInput();
                return;
            }
            tempInt1 = tempInt1 * 10 + (inputBuffer[i] - '0');
            i++;
        }
        clearInput();
        mainLogic.payBillSFML(*currentPatient, tempInt1, outputLines, outputCount);
        currentState = 0;
        return;
    }

    if (currentState == 206)
    {
        float amount = 0.0f;
        int i = 0;
        bool hasDecimal = false;
        float decimalPlace = 0.1f;

        while (inputBuffer[i])
        {
            if (inputBuffer[i] == '.' && !hasDecimal)
                hasDecimal = true;
            else if (inputBuffer[i] >= '0' && inputBuffer[i] <= '9')
            {
                if (!hasDecimal)
                    amount = amount * 10 + (inputBuffer[i] - '0');
                else
                {
                    amount += (inputBuffer[i] - '0') * decimalPlace;
                    decimalPlace *= 0.1f;
                }
            }
            i++;
        }
        clearInput();

        if (amount <= 0)
        {
            addOutput("Invalid amount. Must be greater than 0.");
            return;
        }

        *currentPatient += amount;
        mainLogic.savePatients();
        addOutput("Balance updated successfully.");
        currentState = 0;
        return;
    }

    // ==================== DOCTOR STATES ====================

    if (currentState == 300)
    {
        tempInt1 = 0;
        int i = 0;
        while (inputBuffer[i])
        {
            if (inputBuffer[i] < '0' || inputBuffer[i] > '9')
            {
                addOutput("Invalid ID. Numbers only.");
                clearInput();
                return;
            }
            tempInt1 = tempInt1 * 10 + (inputBuffer[i] - '0');
            i++;
        }
        clearInput();
        mainLogic.markAppointmentCompleteSFML(*currentDoctor, tempInt1, outputLines, outputCount);
        currentState = 0;
        return;
    }

    if (currentState == 301)
    {
        tempInt1 = 0;
        int i = 0;
        while (inputBuffer[i])
        {
            if (inputBuffer[i] < '0' || inputBuffer[i] > '9')
            {
                addOutput("Invalid ID. Numbers only.");
                clearInput();
                return;
            }
            tempInt1 = tempInt1 * 10 + (inputBuffer[i] - '0');
            i++;
        }
        clearInput();
        mainLogic.markAppointmentNoShowSFML(*currentDoctor, tempInt1, outputLines, outputCount);
        currentState = 0;
        return;
    }

    if (currentState == 302)
    {
        tempInt1 = 0;
        int i = 0;
        while (inputBuffer[i])
        {
            if (inputBuffer[i] < '0' || inputBuffer[i] > '9')
            {
                addOutput("Invalid ID. Numbers only.");
                clearInput();
                return;
            }
            tempInt1 = tempInt1 * 10 + (inputBuffer[i] - '0');
            i++;
        }
        clearInput();
        addOutput("Enter medicines:");
        currentState = 3021;
        return;
    }

    if (currentState == 3021)
    {
        int i = 0;
        while (inputBuffer[i]) { tempStr1[i] = inputBuffer[i]; i++; }
        tempStr1[i] = '\0';
        clearInput();
        addOutput("Enter notes:");
        currentState = 3022;
        return;
    }

    if (currentState == 3022)
    {
        int i = 0;
        while (inputBuffer[i]) { tempStr2[i] = inputBuffer[i]; i++; }
        tempStr2[i] = '\0';
        clearInput();
        mainLogic.writePrescriptionSFML(*currentDoctor, tempInt1, tempStr1, tempStr2, outputLines, outputCount);
        currentState = 0;
        return;
    }

    if (currentState == 303)
    {
        tempInt1 = 0;
        int i = 0;
        while (inputBuffer[i])
        {
            if (inputBuffer[i] < '0' || inputBuffer[i] > '9')
            {
                addOutput("Invalid ID. Numbers only.");
                clearInput();
                return;
            }
            tempInt1 = tempInt1 * 10 + (inputBuffer[i] - '0');
            i++;
        }
        clearInput();
        mainLogic.viewPatientMedicalHistorySFML(*currentDoctor, tempInt1, outputLines, outputCount);
        currentState = 0;
        return;
    }

    // ==================== ADMIN STATES ====================

    if (currentState == 400)
    {
        int i = 0;
        while (inputBuffer[i]) { tempStr1[i] = inputBuffer[i]; i++; }
        tempStr1[i] = '\0';
        clearInput();
        addOutput("Enter specialization:");
        currentState = 4001;
        return;
    }

    if (currentState == 4001)
    {
        int i = 0;
        while (inputBuffer[i]) { tempStr2[i] = inputBuffer[i]; i++; }
        tempStr2[i] = '\0';
        clearInput();
        addOutput("Enter contact (11 digits):");
        currentState = 4002;
        return;
    }

    if (currentState == 4002)
    {
        if (!Validator::contactValidity(inputBuffer))
        {
            addOutput("Invalid contact. Must be exactly 11 digits.");
            clearInput();
            return;
        }
        int i = 0;
        while (inputBuffer[i]) { tempStr3[i] = inputBuffer[i]; i++; }
        tempStr3[i] = '\0';
        clearInput();
        addOutput("Enter password (min 6 chars):");
        currentState = 4003;
        return;
    }

    if (currentState == 4003)
    {
        if (!Validator::passwordValidity(inputBuffer))
        {
            addOutput("Invalid password. Must be at least 6 characters.");
            clearInput();
            return;
        }
        addOutput("Enter fee:");
        int i = 0;
        while (inputBuffer[i] && i < 199) { tempStr4[i] = inputBuffer[i]; i++; }
        tempStr4[i] = '\0';
        clearInput();
        currentState = 4004;
        return;
    }

    if (currentState == 4004)
    {
        float fee = 0.0f;
        int i = 0;
        bool hasDecimal = false;
        float decimalPlace = 0.1f;
        while (inputBuffer[i])
        {
            if (inputBuffer[i] == '.' && !hasDecimal) hasDecimal = true;
            else if (inputBuffer[i] >= '0' && inputBuffer[i] <= '9')
            {
                if (!hasDecimal) fee = fee * 10 + (inputBuffer[i] - '0');
                else { fee += (inputBuffer[i] - '0') * decimalPlace; decimalPlace *= 0.1f; }
            }
            i++;
        }
        clearInput();

        if (fee <= 0)
        {
            addOutput("Invalid fee. Must be positive.");
            return;
        }

        mainLogic.addDoctorSFML(tempStr1, tempStr2, tempStr3, tempStr4, fee, outputLines, outputCount);
        currentState = 0;
        return;
    }

    if (currentState == 401)
    {
        tempInt1 = 0;
        int i = 0;
        while (inputBuffer[i])
        {
            if (inputBuffer[i] < '0' || inputBuffer[i] > '9')
            {
                addOutput("Invalid ID. Numbers only.");
                clearInput();
                return;
            }
            tempInt1 = tempInt1 * 10 + (inputBuffer[i] - '0');
            i++;
        }
        clearInput();
        mainLogic.removeDoctorSFML(tempInt1, outputLines, outputCount);
        currentState = 0;
        return;
    }

    if (currentState == 402)
    {
        tempInt1 = 0;
        int i = 0;
        while (inputBuffer[i])
        {
            if (inputBuffer[i] < '0' || inputBuffer[i] > '9')
            {
                addOutput("Invalid ID. Numbers only.");
                clearInput();
                return;
            }
            tempInt1 = tempInt1 * 10 + (inputBuffer[i] - '0');
            i++;
        }
        clearInput();
        mainLogic.dischargePatientSFML(tempInt1, outputLines, outputCount);
        currentState = 0;
        return;
    }
}

void SFMLApp::drawLoginScreen()
{
    sf::Text title = makeText("MediCore Hospital", 280, 100, 36, sf::Color::White);
    sf::Text subtitle = makeText("Management System", 300, 145, 28, sf::Color(200, 200, 200));
    sf::Text prompt = makeText("Login as:", 440, 250, 22, sf::Color(200, 200, 200));

    sf::RectangleShape btn1 = makeButton(387, 300, 250, 50, sf::Color(70, 130, 180));
    sf::RectangleShape btn2 = makeButton(387, 370, 250, 50, sf::Color(70, 130, 180));
    sf::RectangleShape btn3 = makeButton(387, 440, 250, 50, sf::Color(70, 130, 180));
    sf::RectangleShape btn4 = makeButton(387, 510, 250, 50, sf::Color(180, 70, 70));

    sf::Text lbl1 = makeText("Patient", 460, 312, 20, sf::Color::White);
    sf::Text lbl2 = makeText("Doctor", 463, 382, 20, sf::Color::White);
    sf::Text lbl3 = makeText("Admin", 465, 452, 20, sf::Color::White);
    sf::Text lbl4 = makeText("Exit", 468, 522, 20, sf::Color::White);

    window.draw(title); window.draw(subtitle); window.draw(prompt);
    window.draw(btn1); window.draw(lbl1);
    window.draw(btn2); window.draw(lbl2);
    window.draw(btn3); window.draw(lbl3);
    window.draw(btn4); window.draw(lbl4);
}

void SFMLApp::drawPatientMenu()
{
    // SHOW PATIENT NAME AND BALANCE AT TOP
    if (currentPatient != nullptr)
    {
        char info[100] = "Welcome, ";
        int i = 9;
        const char* name = currentPatient->getName();
        int j = 0;
        while (name[j]) { info[i++] = name[j++]; }
        info[i] = '\0';
        sf::Text welcome = makeText(info, 50, 50, 20, sf::Color(70, 200, 130));
        window.draw(welcome);
    }

    sf::Text title = makeText("Patient Menu", 390, 100, 32, sf::Color::White);

    sf::RectangleShape btn1 = makeButton(387, 200, 250, 45, sf::Color(70, 130, 180));
    sf::RectangleShape btn2 = makeButton(387, 255, 250, 45, sf::Color(70, 130, 180));
    sf::RectangleShape btn3 = makeButton(387, 310, 250, 45, sf::Color(70, 130, 180));
    sf::RectangleShape btn4 = makeButton(387, 365, 250, 45, sf::Color(70, 130, 180));
    sf::RectangleShape btn5 = makeButton(387, 420, 250, 45, sf::Color(70, 130, 180));
    sf::RectangleShape btn6 = makeButton(387, 475, 250, 45, sf::Color(70, 130, 180));
    sf::RectangleShape btn7 = makeButton(387, 530, 250, 45, sf::Color(70, 130, 180));
    sf::RectangleShape btn8 = makeButton(387, 585, 250, 45, sf::Color(180, 70, 70));

    sf::Text lbl1 = makeText("Book Appointment", 400, 212, 18, sf::Color::White);
    sf::Text lbl2 = makeText("Cancel Appointment", 400, 267, 18, sf::Color::White);
    sf::Text lbl3 = makeText("View Appointments", 400, 322, 18, sf::Color::White);
    sf::Text lbl4 = makeText("View Medical Records", 400, 377, 18, sf::Color::White);
    sf::Text lbl5 = makeText("View Bills", 400, 432, 18, sf::Color::White);
    sf::Text lbl6 = makeText("Pay Bill", 400, 487, 18, sf::Color::White);
    sf::Text lbl7 = makeText("Top Up Balance", 400, 542, 18, sf::Color::White);
    sf::Text lbl8 = makeText("Logout", 400, 597, 18, sf::Color::White);

    window.draw(title);
    window.draw(btn1); window.draw(lbl1);
    window.draw(btn2); window.draw(lbl2);
    window.draw(btn3); window.draw(lbl3);
    window.draw(btn4); window.draw(lbl4);
    window.draw(btn5); window.draw(lbl5);
    window.draw(btn6); window.draw(lbl6);
    window.draw(btn7); window.draw(lbl7);
    window.draw(btn8); window.draw(lbl8);
}

void SFMLApp::drawDoctorMenu()
{
    if (currentDoctor != nullptr)
    {
        char info[100] = "Dr. ";
        int i = 4;
        const char* name = currentDoctor->getName();
        int j = 0;
        while (name[j]) { info[i++] = name[j++]; }
        info[i] = '\0';
        sf::Text welcome = makeText(info, 50, 50, 20, sf::Color(70, 200, 130));
        window.draw(welcome);
    }

    sf::Text title = makeText("Doctor Menu", 390, 100, 32, sf::Color::White);

    sf::RectangleShape btn1 = makeButton(387, 250, 250, 45, sf::Color(70, 130, 180));
    sf::RectangleShape btn2 = makeButton(387, 305, 250, 45, sf::Color(70, 130, 180));
    sf::RectangleShape btn3 = makeButton(387, 360, 250, 45, sf::Color(70, 130, 180));
    sf::RectangleShape btn4 = makeButton(387, 415, 250, 45, sf::Color(70, 130, 180));
    sf::RectangleShape btn5 = makeButton(387, 470, 250, 45, sf::Color(70, 130, 180));
    sf::RectangleShape btn6 = makeButton(387, 525, 250, 45, sf::Color(180, 70, 70));

    sf::Text lbl1 = makeText("Today's Appointments", 400, 262, 18, sf::Color::White);
    sf::Text lbl2 = makeText("Mark Complete", 400, 317, 18, sf::Color::White);
    sf::Text lbl3 = makeText("Mark No-Show", 400, 372, 18, sf::Color::White);
    sf::Text lbl4 = makeText("Write Prescription", 400, 427, 18, sf::Color::White);
    sf::Text lbl5 = makeText("Patient Medical History", 400, 482, 18, sf::Color::White);
    sf::Text lbl6 = makeText("Logout", 400, 537, 18, sf::Color::White);

    window.draw(title);
    window.draw(btn1); window.draw(lbl1);
    window.draw(btn2); window.draw(lbl2);
    window.draw(btn3); window.draw(lbl3);
    window.draw(btn4); window.draw(lbl4);
    window.draw(btn5); window.draw(lbl5);
    window.draw(btn6); window.draw(lbl6);
}

void SFMLApp::drawAdminMenu()
{
    sf::Text title = makeText("Admin Panel", 390, 80, 32, sf::Color::White);

    sf::RectangleShape btn1 = makeButton(387, 150, 250, 40, sf::Color(70, 130, 180));
    sf::RectangleShape btn2 = makeButton(387, 200, 250, 40, sf::Color(70, 130, 180));
    sf::RectangleShape btn3 = makeButton(387, 250, 250, 40, sf::Color(70, 130, 180));
    sf::RectangleShape btn4 = makeButton(387, 300, 250, 40, sf::Color(70, 130, 180));
    sf::RectangleShape btn5 = makeButton(387, 350, 250, 40, sf::Color(70, 130, 180));
    sf::RectangleShape btn6 = makeButton(387, 400, 250, 40, sf::Color(70, 130, 180));
    sf::RectangleShape btn7 = makeButton(387, 450, 250, 40, sf::Color(70, 130, 180));
    sf::RectangleShape btn8 = makeButton(387, 500, 250, 40, sf::Color(70, 130, 180));
    sf::RectangleShape btn9 = makeButton(387, 550, 250, 40, sf::Color(70, 130, 180));
    sf::RectangleShape btn10 = makeButton(387, 600, 250, 40, sf::Color(180, 70, 70));

    sf::Text lbl1 = makeText("Add Doctor", 400, 160, 18, sf::Color::White);
    sf::Text lbl2 = makeText("Remove Doctor", 400, 210, 18, sf::Color::White);
    sf::Text lbl3 = makeText("View All Patients", 400, 260, 18, sf::Color::White);
    sf::Text lbl4 = makeText("View All Doctors", 400, 310, 18, sf::Color::White);
    sf::Text lbl5 = makeText("View All Appointments", 400, 360, 18, sf::Color::White);
    sf::Text lbl6 = makeText("View Unpaid Bills", 400, 410, 18, sf::Color::White);
    sf::Text lbl7 = makeText("Discharge Patient", 400, 460, 18, sf::Color::White);
    sf::Text lbl8 = makeText("View Security Log", 400, 510, 18, sf::Color::White);
    sf::Text lbl9 = makeText("Generate Daily Report", 400, 560, 18, sf::Color::White);
    sf::Text lbl10 = makeText("Logout", 400, 610, 18, sf::Color::White);

    window.draw(title);
    window.draw(btn1);  window.draw(lbl1);
    window.draw(btn2);  window.draw(lbl2);
    window.draw(btn3);  window.draw(lbl3);
    window.draw(btn4);  window.draw(lbl4);
    window.draw(btn5);  window.draw(lbl5);
    window.draw(btn6);  window.draw(lbl6);
    window.draw(btn7);  window.draw(lbl7);
    window.draw(btn8);  window.draw(lbl8);
    window.draw(btn9);  window.draw(lbl9);
    window.draw(btn10); window.draw(lbl10);
}