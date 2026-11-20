//For the GUI
#include <raylib.h>
//For randomizing first turn of the game and as well as to be used in Easy Computer gamemode
#include <cstdlib>
#include <ctime>
//For Harder Computer gamemode by introducing algorithm
#include <algorithm>
#include <limits>
//To code JSON files much easier
#include "json.hpp"
#include <fstream>
//For testing JSON file only
#include <iostream>

//call out nlohmann/json.hpp
using json = nlohmann::json;

//Functions========================================================================================================
//Check who wins and also if the game is not done yet
char checker(char ttt[3][3]) {
    //check if the table have any rows that are identical to each other 
    for (int row = 0; row < 3; row++) {
        if (ttt[row][0] == ttt[row][1] && ttt[row][1] == ttt[row][2] && ttt[row][0] != ' ') {
            return ttt[row][0];
        }
    }

    //check if the table have any column that are identical to each other
    for (int col = 0; col < 3; col++) {
        if (ttt[0][col] == ttt[1][col] && ttt[1][col] == ttt[2][col] && ttt[0][col] != ' ') {
            return ttt[0][col];
        }
    }

    //check if the table have any diagonal that are identical to each other  
    if (ttt[0][0] == ttt[1][1] && ttt[1][1] == ttt[2][2] && ttt[0][0] != ' ') {
        return ttt[0][0];  
    }
    if (ttt[0][2] == ttt[1][1] && ttt[1][1] == ttt[2][0] && ttt[0][2] != ' ') {
        return ttt[0][2];
    }

    return ' ';
}

//This checks if the board is full and consider it as a tie
bool fullBoard(char ttt[3][3]) {
    //this counts the row and col
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            //this checks every empty box
            if (ttt[row][col] == ' ') {
                return false; //this tells to the main that all the box is not full yet
            }
        }
    }
    return true; // this tells that the box is full now and it is a tie
}

//Resetting the board
void Resetting(char ttt[3][3]) {
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            ttt[row][col] = ' ';
        }
    }
}

//Algorithm-----------------------------------------------------------------------------
//Thinking function
int minimax(char ttt[3][3], int depth, bool isAiTurn) {
    //it uses the cheker function to find any winning move
    int score = checker(ttt);
    //This checks if the computer wins, losses or tie with the simulated player and gets a point or not, depending to the final simulated game
    if (score == 'X') return 10;
    if (score == 'O') return -10;
    if (fullBoard(ttt)) return 0;

    //If isAuTurn is true
    if (isAiTurn) {
        //Place holder for te BestScore
        int bestScore = -1000;
        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 3; col++) {
                if (ttt[row][col] == ' ') {
                    //Trying a move
                    ttt[row][col] = 'X';
                    //Recursive (to loop inside itself)
                    int cScore = minimax(ttt, depth + 1, false);
                    //This won't execute until the Recursive is done
                    //Remove the tried move
                    ttt[row][col] = ' ';
                    //Pick the current Score and replace it to the bestscore
                    bestScore = std::max(bestScore, cScore);
                }
            }
        }
        //Returning the bestScore value out 
        return bestScore;                         
    } 
    //If isAuTurn is false
    else {
        //Place holder for te BestScore
        int bestScore = 1000;
        //
        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 3; col++) {
                if (ttt[row][col] == ' ') {
                    //Trying the move
                    ttt[row][col] = 'O';
                    //Recursive (to loop inside itself)
                    int cScore = minimax(ttt, depth + 1, true);
                    //This won't execute until the Recursive is done
                    //Remove the tried move
                    ttt[row][col] = ' ';
                    //Pick the current Score and replace it to the bestscore
                    bestScore = std::min(bestScore, cScore);
                }
            }
        }
        //Returning the bestScore value out 
        return bestScore;
    }
}

//Decision maker
//&bRow and &bCol means to directly manipulate the row and col outside of this function
void findBestMove (char ttt[3][3], int &bRow, int &bCol) {
    //Placeholders
    int bestV = -1000;
    bRow = -1;
    bCol = -1;

    //reads the table
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            if (ttt[row][col] == ' ') {
                //Trying a move
                ttt[row][col] = 'X';
                //Talk with minimax function so that it confirms if it really the best action
                int moveV = minimax(ttt, 0, false);
                //After thinking it remove the tried move
                ttt[row][col] = ' ';

                //It then apply the values of the bestmove as well the row and column of it
                if (moveV > bestV) {
                    bestV = moveV;
                    bRow = row;
                    bCol = col;
                }
            }
        }
    }
}

//Just for the details
const char* instructions[] = {
    "Welcome to Tic-Tac-Toe!",
    "1.) Choose your game mode by typing 1 or 2:",
    "2.) Type 1 to play against the Computer.",
    "3.) Type 2 to play against a 2nd Player (you will take turns",
    "making moves).",
    "4.) The game will randomly pick who goes first.",
    "5.) Click on any empty box to place your mark.",
    "6.) In Player vs. Computer mode, the computer will make its move",
    "automatically.",
    "7.) A match lasts for 5 rounds, after which the game will declare",
    "the winner or a tie.",
    "8.) If you beat the Computer, your win counter (top-left) will go",
    "up points",
    "awarded depend on the difficulty (Easy or Hard).",
    "9.) Most importantly, have fun!"
};

const char* team[4][4] = {
    {"RJ","Gabas"},
    {"Gela","Lagarde"},
    {"Dianne", "DelaCruz"},
    {"Jed", "Austria"}
};

//Functions========================================================================================================

//Objects==========================================================================================================

//Instruct what the button should have
struct btn {
    int x, y, width, height, paddingX, paddingY, fontSize;
    const char* text;
    Color color;
};

//This is the button class that will be used to create buttons in the game
class Button {
    public:
        btn button;

        //it tells where and what should you put in a button
        Button(int x, int y, int width, int height, const char* text, int paddingX, int paddingY, int fontSize, Color color) {
            button.x = x;
            button.y = y;
            button.width = width;
            button.height = height;
            button.text = text;
            button.paddingX = paddingX;
            button.paddingY = paddingY;
            button.fontSize = fontSize;
            button.color = color;
        }

        //when tells to draw it
        void draw() {
            DrawRectangle(button.x, button.y, button.width, button.height, button.color);
            DrawText(button.text, button.x + button.paddingX, button.y + button.paddingY, button.fontSize, BLACK);
            DrawRectangleLines(button.x, button.y, button.width, button.height, BLACK);
        }

        //event listener
        bool isClicked() {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), (Rectangle){(float)button.x, (float)button.y, (float)button.width, (float)button.height})) {
                return true;
            }
            return false;
        } 
};

//-------------------------------------------------------------------------------------------------------

struct tb {
    int x, y, width, height, spacing;
    const char* text;
    Color color;
};

class TableBoxes {
    public:
        tb box;

        TableBoxes(int x, int y, int width, int height, int spacing, const char* text) {
            box.x = x;
            box.y = y;
            box.width = width;
            box.height = height;
            box.text = text;
            box.spacing = spacing;

            box.color = (Color){255, 255, 255, 20};
        }

        void draw(char ttt[3][3]) {
            for (int row = 0; row < 3; row++) {
                for (int col = 0; col < 3; col++){
                    int xAxis = box.x + col * (box.width + box.spacing);
                    int yAxis = box.y + row * (box.height + box.spacing);

                    DrawRectangle(xAxis, yAxis, box.width, box.height, box.color);
                    DrawRectangleLines(xAxis, yAxis, box.width, box.height, WHITE);

                    if(ttt[row][col] == 'X') {
                        DrawText("X", xAxis + 30, yAxis + 20, 60, GREEN);
                    } else if (ttt[row][col] == 'O'){
                        DrawText("O", xAxis + 30, yAxis + 20, 60, RED);
                    }
                }
            }
        }

        bool isClicked(char ttt[3][3], char mark) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mousePos = GetMousePosition();
                for (int row = 0; row < 3; row++) {
                    for (int col = 0; col < 3; col++) {
                        int xAxis = box.x + col * (box.width + box.spacing);
                        int yAxis = box.y + row * (box.height + box.spacing);

                        if (CheckCollisionPointRec(mousePos, (Rectangle){(float)xAxis, (float)yAxis, (float)box.width, (float)box.height})) {
                            if (ttt[row][col] == ' ') {
                                ttt[row][col] = mark;
                                return true;
                            }
                        }
                    }
                }
            }
        return false;
        }
};

//-------------------------------------------------------------------------------------------------------

struct UIbox {
    int x, y, width, height, paddingX, paddingY, fontSize;
    char text[11];
    const char* placeholder;
    Color color;
};

class UserInput {
public:
    UIbox UI;
    bool isActive;

    UserInput(int x, int y, int width, int height, int paddingX, int paddingY, int fontSize, const char* placeholder) {
        UI.x = x;
        UI.y = y;
        UI.width = width;
        UI.height = height;
        UI.paddingX = paddingX;
        UI.paddingY = paddingY;
        UI.fontSize = fontSize;
        UI.placeholder = placeholder;
        
        // Clear the text array so it starts completely empty
        for (int i = 0; i < 11; i++) {
            UI.text[i] = '\0';
        }
        
        isActive = false;
    }

    // Call this every frame to check for clicks and typing
    void isClicked() {
        //if clicked
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Rectangle boxRect = {(float)UI.x, (float)UI.y, (float)UI.width, (float)UI.height};
            if (CheckCollisionPointRec(GetMousePosition(), boxRect)) {
                isActive = true;  
            } else {
                isActive = false;
            }
        }

        // 2. If the box is active, let the user type letters into it
        if (isActive) {
            int key = GetCharPressed();

            // Find how many letters are currently typed
            int currentLength = 0;
            while (currentLength < 10 && UI.text[currentLength] != '\0') {
                currentLength++;
            }

            // If a valid key was pressed and we haven't hit the 10-letter limit
            if (key >= 32 && key <= 125 && currentLength < 10) {
                UI.text[currentLength] = (char)key;
                UI.text[currentLength + 1] = '\0';
            }

            // Handle hitting Backspace to delete letters
            if (IsKeyPressed(KEY_BACKSPACE)) {
                if (currentLength > 0) {
                    UI.text[currentLength - 1] = '\0';
                }
            }
        }
    }

    void draw() {
        Color boxColor = RAYWHITE;
        if (isActive) {
            boxColor = LIGHTGRAY;
        }

        // Draw box background and outline
        DrawRectangle(UI.x, UI.y, UI.width, UI.height, boxColor);
        DrawRectangleLines(UI.x, UI.y, UI.width, UI.height, BLACK);

        if (UI.text[0] == '\0') {
        // if Empty
        DrawText(UI.placeholder, UI.x + UI.paddingX, UI.y + UI.paddingY, UI.fontSize, LIGHTGRAY);
        } else {
        // If full
        DrawText(UI.text, UI.x + UI.paddingX, UI.y + UI.paddingY, UI.fontSize, BLACK);
        }
    }
};

//=================================================================================================================

//main program
int main() {
    //Variables==========================================================================================
    //The Table of the game
    char ttt[3][3] = {
        {' ', ' ', ' '}, //First row
        {' ', ' ', ' '}, //Second row
        {' ', ' ', ' '}  //Third row
    };

    // Call the UserInfo JSON File
    std::fstream file("src/UserInfo.JSON"); //Bring file here and read or file out

    //It just look if the file is there or not (SO IGNORE!!!)----------------
    if (!file.is_open()) {
        std::cerr << "ERROR: Cannot find UserInfo.JSON!" << std::endl;
        return 1;
    }

    json store;

    try {
        file >> store;
    }
    catch (const json::parse_error& e) {
        std::cerr << "JSON ERROR: " << e.what() << std::endl;
        return 1;
    }

    file.close();
    //-------------------------------------------------------------------------

    //For numbers to be randomized
    srand(time(0));

    //Get mouse current position
    Vector2 mousePosition = GetMousePosition();

    //get postion
    int startY = 100;
    int lineSpacing = 30;

    //Background dimensions
    const int widthBackground = 800;
    const int heightBackground = 600;

    //Colors used
    Color backgroundColor = (Color){ 10, 10, 30, 255};
    Color menuBoard = (Color){ 20, 20, 50, 255};

    //Screen Variables
    int currentScreen = 0;

    //Register variable
    bool alreadyMade = false;
    bool notMade = false;
    int easterEgg = 0;

    //All variables for the tictoctoe
    int currentTurn = rand() % 2; //randomize the first turn
    int currentRound = 1;
    int Player1, Player2, Computer, fought;
    bool isPaused = false;
    bool scoreUpdate = false;
    bool isHardmode = false;
    float thinking = 0.0f;
    

    std::string readUsername;
    std::string readPassword;

    //Assets==================================================================================================
    //Login and register Items
    UserInput username(150, heightBackground / 2 - 110, 500, 70, 20, 20, 45, "Username:");
    UserInput password(150, heightBackground / 2 - 30, 500, 70, 20, 20, 45, "Password:");

    Button logging(150, heightBackground / 2 + 50, 500, 70, "Log In", 180, 20, 45, WHITE);
    Button registering(150, heightBackground / 2 + 130, 500, 70, "Register", 160, 20, 45, WHITE);
    Button exitLoginRegister(widthBackground - 150, 30, 130, 50, "Exit", 30, 8, 36, WHITE);

    Button registerbtn(150, heightBackground / 2 + 50, 500, 70, "Register Confirm", 50, 20, 45, WHITE);
    Button goBackToLogin(150, heightBackground / 2 + 130, 500, 70, "Go Back", 160, 20, 45, WHITE);
    //--------------------------------------------------------------------------------------------------------
    //buttons in the main menu
    Button startButton(widthBackground / 2 - 170, heightBackground / 2 - 90, 320, 80, "Start Game", 30, 23, 40, WHITE);
    Button aboutButton(widthBackground / 2 - 170, heightBackground / 2 + 20, 320, 80, "About", 30, 23, 40, WHITE);
    Button exitButton(widthBackground / 2 - 170, heightBackground / 2 + 130, 320, 80, "Exit Game", 30, 23, 40, WHITE);
    Button exitGamemodeButton(widthBackground / 2 - 250, heightBackground / 2 + 120, 500, 80, "Go Back", 30, 23, 30, WHITE);

    //--------------------------------------------------------------------------------------------------------
    //Game mode Selection buttons
    Button computerEasyButton(widthBackground / 2 - 250, heightBackground / 2 - 180, 500, 80, "Player VS Computer (Easy)", 30, 23, 30, WHITE);
    Button computerHardButton(widthBackground / 2 - 250, heightBackground / 2 -80, 500, 80, "Player VS Computer (Hard)", 30, 23, 30, WHITE);
    Button pvpButton(widthBackground / 2 - 250, heightBackground / 2 + 20, 500, 80, "Player VS Player", 30, 23, 30, WHITE);

    //--------------------------------------------------------------------------------------------------------
    //Universal go back buttons
    Button gobackButton(widthBackground - 210, heightBackground - 80, 180, 70, "Go Back", 30, 23, 30, WHITE);

    //--------------------------------------------------------------------------------------------------------
    //Actual Game
    TableBoxes table(widthBackground / 2 - 180, heightBackground / 2 - 150, 100, 100, 20, "x");

    //========================================================================================================

    InitWindow(widthBackground, heightBackground, "Tic Tac Toe");
    SetTargetFPS(60);

    while (!WindowShouldClose() && currentScreen != 9) {
        //update the background
        ClearBackground(backgroundColor);

        //Update==========================================================================================
        mousePosition = GetMousePosition();
        switch (currentScreen) {
            //Login--------------------------------------------------------------------
            case 0: {
                username.isClicked();
                password.isClicked();

                if (logging.isClicked()) {
                    readUsername = username.UI.text;
                    readPassword = password.UI.text;
                
                    for (size_t read = 0; read < store.size(); read++) {
                        if (store[read]["Username"] == readUsername && store[read]["Password"] == readPassword) {
                            notMade = false;
                            currentScreen = 2;

                            //Easter eggs!
                            if (store[read]["Username"] == "Lester" && store[read]["Password"] == "Casalta") {
                                easterEgg = 1;
                            }
                            for (int num = 0; num < 4; num++) {
                                if (store[read]["Username"] == team[num][0] && store[read]["Password"] == team[num][1]) {
                                    easterEgg = 2;
                                }
                            }
                            if (store[read]["Username"] == "Richie" && store[read]["Password"] == "Manreal") {
                                easterEgg = 3;
                            }
                        } else {
                            notMade = true;
                        }
                    }
                }

                if (registering.isClicked()) {
                    //Flush out the inputted text in the Username and Password bar
                    username.UI.text[0] = '\0';
                    password.UI.text[0] = '\0';
                    notMade = false;
                    currentScreen = 1;
                }
                if (exitLoginRegister.isClicked()) {currentScreen = 9;}

                break;
            }
            //Create-------------------------------------------------------------------
            case 1: {
                username.isClicked();
                password.isClicked();

                if (registerbtn.isClicked()) {
                    readUsername = username.UI.text;
                    readPassword = password.UI.text;

                    if (readUsername.empty() || readPassword.empty()) {
                        alreadyMade = true;
                    }

                    for (size_t read = 0; read < store.size(); read++) {
                        if (store[read]["Username"] == readUsername && store[read]["Password"] == readPassword) {
                            alreadyMade = true;
                        }
                        if (store[read]["Username"] == readUsername) {
                            alreadyMade = true;
                        }
                    }


                    if (!alreadyMade) {
                         store.push_back({
                            {"Username", readUsername},
                            {"Password", readPassword},
                            {"Score(easy)", 0},
                            {"Score(hard)", 0}
                        });

                        std::ofstream fileOut("src/UserInfo.JSON");
                        fileOut << store.dump(4);
                        fileOut.close();
                        username.UI.text[0] = '\0';
                        password.UI.text[0] = '\0';
                        currentScreen = 0;
                    }
                }

                if (goBackToLogin.isClicked()) {
                    username.UI.text[0] = '\0';
                    password.UI.text[0] = '\0';
                    alreadyMade = false;
                    currentScreen = 0;
                }
                if (exitLoginRegister.isClicked()) {currentScreen = 9;}
                break;
            }
            //Main Menu----------------------------------------------------------------
            case 2: {
                //turn everything to default state
                currentRound = 1;
                Player1 = 0;
                Player2 = 0;
                Computer = 0;
                fought = 0;
                scoreUpdate = false;
                isHardmode = false;
                //to make sure that the board is really empty
                Resetting(ttt);
                //if one of the button is clicked then it will go to the selected next screen
                if (startButton.isClicked()) {currentScreen = 3;}
                if (aboutButton.isClicked()) {currentScreen = 4;}
                if (exitButton.isClicked()) {currentScreen = 9;}
                break;
            }
            //Select Game Mode--------------------------------------------------------
            case 3: {
                if (computerEasyButton.isClicked()) {currentScreen = 5;}
                if (computerHardButton.isClicked()) {currentScreen = 6;}
                if (pvpButton.isClicked()) {currentScreen = 7;}
                if (exitGamemodeButton.isClicked()) {currentScreen = 2;}
                break;
            }
            //About-------------------------------------------------------------------
            case 4: {
                if (gobackButton.isClicked()) {currentScreen = 2;}
                break;
            }
            //Player vs Computer (Easy)-----------------------------------------------
            case 5: {
                if (currentRound < 6) {
                    //check if the board has a winner or full
                    char winner = checker(ttt);
                    if (winner != ' ' || fullBoard(ttt)) {
                        if (winner == 'O') {
                            Player1 += 1;
                        } else if (winner == 'X') {
                            Computer += 1;
                        }

                        //add a point in the round and as well as to reset the board
                        currentRound += 1;
                        Resetting(ttt);
                    }
                    //Player>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
                    if (currentTurn == 0) {
                        if (table.isClicked(ttt, 'O')) {
                            currentTurn = 1;
                        }
                    } 
                    //Computer>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
                    else if (currentTurn == 1) {

                        //to let the computer "Think" for a second
                        if (!isPaused) {
                            isPaused = true;
                            thinking = 1.0f; 
                        }

                        if (thinking > 0.0f) {
                            thinking -= GetFrameTime();
                            break;
                        }
                        isPaused = false;
                        //-----------------------------------------

                        while (true) {
                            int aiRow = rand() % 3;
                            int aiCol = rand() % 3;

                            if (ttt[aiRow][aiCol] == ' ') {
                                ttt[aiRow][aiCol] = 'X';
                                break;
                            }
                        }
                        currentTurn = 0;
                    }
                } else {
                    fought = 1;
                    currentScreen = 8;
                }
                break;
            }
            //Player vs Computer (Hard)-----------------------------------------------
            case 6: {
                if (currentRound < 6) {
                    char winner = checker(ttt);
                    if (winner != ' ' || fullBoard(ttt)) {
                        if (winner == 'O') {
                            Player1 += 1;
                        } else if (winner == 'X') {
                            Computer += 1;
                        }

                        currentRound += 1;
                        Resetting(ttt);
                    }
                    //Player>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
                    if (currentTurn == 0) {
                        if (table.isClicked(ttt, 'O')) {
                            currentTurn = 1;
                        }
                    } 
                    //Computer>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
                    else if (currentTurn == 1) {
                        //to let the computer "Think" for a second
                        if (!isPaused) {
                            isPaused = true;
                            thinking = 1.0f; 
                        }

                        if (thinking > 0.0f) {
                            thinking -= GetFrameTime();
                            break;
                        }
                        isPaused = false;
                        //-----------------------------------------

                        int aiRow = -1; 
                        int aiCol = -1;
                        
                        if (currentRound >= 1 && currentRound <= 3) {
                            int balancer = rand() % 5;
                            if (balancer == 4) {
                                while (true) {
                                    aiRow = rand() % 3;
                                    aiCol = rand() % 3;
                                    if (ttt[aiRow][aiCol] == ' ') {
                                        break;
                                    }
                                }
                            } else {findBestMove(ttt, aiRow, aiCol);}
                        } else {findBestMove(ttt, aiRow, aiCol);}

                        if (aiRow != -1 && aiCol != -1 && ttt[aiRow][aiCol] == ' ') {
                            ttt[aiRow][aiCol] = 'X';
                        }
                        currentTurn = 0;
                    }
                } else {
                    isHardmode = true;
                    fought = 1;
                    currentScreen = 8;
                }
                break;
            }
            //Player vs Player--------------------------------------------------------
            case 7: {
                if (currentRound < 6) {
                    char winner = checker(ttt);
                    if (winner != ' ' || fullBoard(ttt)) {
                        if (winner == 'O') {
                            Player1 += 1;
                        } else if (winner == 'X') {
                            Player2 += 1;
                        }
                        currentRound += 1;
                        Resetting(ttt);
                    }
                    //Player1>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
                    if (currentTurn == 0) {
                        if (table.isClicked(ttt, 'O')) {
                            currentTurn = 1;
                        }
                    } 
                    //Player2>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
                    else if (currentTurn == 1) {
                        if (table.isClicked(ttt, 'X')) {
                            currentTurn = 0;
                        }
                    }
                } else {
                    fought = 2;
                    currentScreen = 8;
                }
                break;
            }

            //Result------------------------------------------------------------------
            case 8: {
                if (gobackButton.isClicked()) {
                    readUsername = username.UI.text;
                    readPassword = password.UI.text;

                    //Add a point if the opponent the computer
                    if (Player1 > Computer && !scoreUpdate) {
                        for (size_t read = 0; read < store.size(); read++) {
                            if (store[read]["Username"] == std::string(readUsername) && 
                                store[read]["Password"] == std::string(readPassword)) {
                                
                                int currentScoreEasy = store[read]["Score(easy)"].get<int>();
                                int currentScoreHard = store[read]["Score(hard)"].get<int>();
                                if (!isHardmode) {
                                    store[read]["Score(easy)"] = currentScoreEasy + 1;
                                } else {
                                    store[read]["Score(hard)"] = currentScoreHard + 1;
                                }

                                std::ofstream fileOut("src/UserInfo.JSON");
                                fileOut << store.dump(4);
                                fileOut.close();
                                
                                scoreUpdate = true;
                                break;
                            }
                        }
                    }
                    currentScreen = 2;
                }
                break;
            }
        }
        
        //Drawing=========================================================================================
        BeginDrawing();

        switch (currentScreen) {
            //Login--------------------------------------------------------------------
            case 0: {
                //title of the game in the top center of the window
                DrawText("Tic Tac Toe", widthBackground / 2 - 170, 20, 50, WHITE);
                DrawText("Game", widthBackground / 2 - 80, 70, 50, WHITE);

                //Draw the board
                DrawRectangle(100, heightBackground / 2 - 140, 600, 370, menuBoard);

                //If Login is empty or incorrect account input then remind the user
                if (notMade) {
                    DrawText("Please write something or type the correct Username and Password.", 10, heightBackground - 40, 22, RED);
                }

                //Buttons
                username.draw();
                password.draw();
                logging.draw();
                registering.draw();
                exitLoginRegister.draw();
                break;
            }
            //Create-------------------------------------------------------------------
            case 1: {
                //title of the game in the top left corner of the window
                DrawText("Tic Tac Toe Game", widthBackground / 2 - 230, 20, 50, WHITE);

                //Draw the board
                DrawRectangle(100, heightBackground / 2 - 140, 600, 370, menuBoard);

                //If Register is empty or already have an account input then remind the user
                if (alreadyMade) {
                    DrawText("You typed a existing Username or Account already.", 10, heightBackground - 60, 20, RED);
                    DrawText("Please make a different one.", 10, heightBackground - 30, 20, RED);
                }

                //buttons
                username.draw();
                password.draw();
                registerbtn.draw();
                goBackToLogin.draw();
                exitLoginRegister.draw();

                break;
            }

            //Main Menu----------------------------------------------------------------
            case 2: {
                //title of the game in the top center of the window
                DrawText("Tic Tac Toe", widthBackground / 2 - 170, 20, 50, WHITE);
                DrawText("Game", widthBackground / 2 - 80, 70, 50, WHITE);

                //Draw the menu board
                DrawRectangle(widthBackground / 2 - 185, heightBackground / 2 - 110, 350, 350, menuBoard);

                //Buttons
                startButton.draw();
                aboutButton.draw();
                exitButton.draw();

                //The amount of score that the user won against Computer
                for (size_t read = 0; read < store.size(); read++) {
                    int scoreEasy = store[read]["Score(easy)"].get<int>();
                    int scoreHard = store[read]["Score(hard)"].get<int>();
                    if (store[read]["Username"] == readUsername) {
                        DrawText(TextFormat("Wins(Easy): %i", scoreEasy), 20, 20, 25, YELLOW);
                        DrawText(TextFormat("Wins(Hard): %i", scoreHard), 20, 50, 25, YELLOW);
                    }
                }

                //easter egg
                if (easterEgg == 1) {
                    DrawText("Hello Creator!", 10, heightBackground - 50, 40, WHITE);
                }
                if (easterEgg == 2) {
                    DrawText("Hello Creator's Groupmates!", 10, heightBackground - 50, 35, WHITE);
                }
                if (easterEgg == 3) {
                    DrawText("Hello Creator's Professor!", 10, heightBackground - 50, 35, WHITE);
                }
                break;
            }
            //Select Game Mode--------------------------------------------------------
            case 3: {
                //Select Game Mode
                //title of the game in the top left corner of the window
                DrawText("Tic Tac Toe Game", widthBackground / 2 - 230, 20, 50, WHITE);

                //Draw the menu board
                DrawRectangle(widthBackground / 2 - 300, heightBackground / 2 - 200, 600, 430, menuBoard);

                //Buttons
                computerEasyButton.draw();
                computerHardButton.draw();
                pvpButton.draw();
                exitGamemodeButton.draw();
                break;
            }
            //About-------------------------------------------------------------------
            case 4: {
                //title of the game in the top left corner of the window
                DrawText("Tic Tac Toe Game", widthBackground / 2 - 230, 20, 50, WHITE);

                //Placing each sentences unifiedly
                for (int col = 0; col < 16; col++) {
                    DrawText(instructions[col], 20, startY + (col * lineSpacing), 21, WHITE);
                }

                //Button
                gobackButton.draw();
                break;
            }
            //Player vs Computer (Easy)-----------------------------------------------
            case 5: {
                //title of the game in the top left corner of the window
                DrawText("Tic Tac Toe", 30, 20, 26, WHITE);
                DrawText("Game", 30, 45, 26, WHITE);

                //Tells the Round
                DrawText(TextFormat("Round: %i", currentRound), widthBackground / 2 - 100, 60, 45, WHITE);

                //Marks
                DrawText("O", 70, heightBackground / 2 - 50, 100, RED);
                DrawText("X", widthBackground - 140, heightBackground / 2 - 50, 100, GREEN);

                //Draw the Table
                table.draw(ttt);

                //Tells the user/s who's turn
                if (currentTurn == 0) {
                    DrawRectangle(70, heightBackground / 2 + 40, 60, 10, WHITE);
                } else if (currentTurn == 1) {
                    DrawRectangle(widthBackground - 140, heightBackground / 2 + 40, 60, 10, WHITE);
                    DrawText("Thinking...", 20, heightBackground - 80, 50, WHITE);
                }
                break;
            }
            //Player vs Computer (Hard)-----------------------------------------------
            case 6: {
                //title of the game in the top left corner of the window
                DrawText("Tic Tac Toe", 30, 20, 26, WHITE);
                DrawText("Game", 30, 45, 26, WHITE);

                //Tells the Round
                DrawText(TextFormat("Round: %i", currentRound), widthBackground / 2 - 100, 60, 45, WHITE);

                //Marks
                DrawText("O", 70, heightBackground / 2 - 50, 100, RED);
                DrawText("X", widthBackground - 140, heightBackground / 2 - 50, 100, GREEN);

                //Draw the Table
                table.draw(ttt);

                //Tells the user/s who's turn
                if (currentTurn == 0) {
                    DrawRectangle(70, heightBackground / 2 + 40, 60, 10, WHITE);
                } else if (currentTurn == 1) {
                    DrawRectangle(widthBackground - 140, heightBackground / 2 + 40, 60, 10, WHITE);
                    DrawText("Thinking...", 20, heightBackground - 80, 50, WHITE);
                }

                break;
            }
            //Player vs Player--------------------------------------------------------
            case 7: {
                //title of the game in the top left corner of the window
                DrawText("Tic Tac Toe", 30, 20, 26, WHITE);
                DrawText("Game", 30, 45, 26, WHITE);

                //Tells the Round
                DrawText(TextFormat("Round: %i", currentRound), widthBackground / 2 - 100, 60, 45, WHITE);

                //Marks
                DrawText("O", 70, heightBackground / 2 - 50, 100, RED);
                DrawText("X", widthBackground - 140, heightBackground / 2 - 50, 100, GREEN);

                //Draw the Table
                table.draw(ttt);

                //Tells the user/s who's turn
                if (currentTurn == 0) {
                    DrawRectangle(70, heightBackground / 2 + 40, 60, 10, WHITE);
                } else if (currentTurn == 1) {
                    DrawRectangle(widthBackground - 140, heightBackground / 2 + 40, 60, 10, WHITE);
                }
                break;
            }
            //Game Over---------------------------------------------------------------
            case 8: {
                //title of the game in the top left corner of the window
                DrawText("Tic Tac Toe Game", widthBackground / 2 - 230, 20, 50, WHITE);

                //When the user picks either easy or hard Computer
                if (fought ==  1) {

                    DrawRectangle(widthBackground / 2 - 300, 140, 600, 300, menuBoard);

                    //Show the scores
                    DrawText(TextFormat("Player = %i", Player1), 120, heightBackground / 2 + 40, 40, WHITE);
                    DrawText(TextFormat("Computer = %i", Computer), widthBackground / 2 + 20, heightBackground / 2 + 40, 40, WHITE);

                    //Check who wins or it is a tie
                    if (Player1 == Computer) {
                        DrawText("TIE!", 230, 150, 170, WHITE);
                    } else if (Player1 > Computer) {
                        DrawText("PLAYER WINS!", 135, 200, 70, WHITE);
                    } else if (Player1 < Computer) {
                        DrawText("COMPUTER WINS!", 100, 200, 68, WHITE);
                    }
                
                //When the user pick Player vs Player
                } else if (fought == 2) {

                    DrawRectangle(widthBackground / 2 - 300, 140, 600, 300, menuBoard);

                    //Show the scores
                    DrawText(TextFormat("Player1 = %i", Player1), 120, heightBackground / 2 + 40, 40, WHITE);
                    DrawText(TextFormat("Player2 = %i", Player2), widthBackground / 2 + 30, heightBackground / 2 + 40, 40, WHITE);

                    //Check who wins or it is a tie
                    if (Player1 == Player2) {
                        DrawText("TIE!", 230, 150, 170, WHITE);
                    } else if (Player1 > Player2) {
                        DrawText("PLAYER1 WINS!", 130, 200, 70, WHITE);
                    } else if (Player1 < Player2) {
                        DrawText("PLAYER2 WINS!", 130, 200, 70, WHITE);
                    }
                }
                //Button
                gobackButton.draw();
                break;
            }
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}