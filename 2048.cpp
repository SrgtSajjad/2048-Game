#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>
#include <fstream>
using namespace std;

struct Profile // for creating and saving player data
{
    string name;
    int score;
    int size;
};

/*Colors*/
const string RESET_PAINT = "\033[0m";
const string BLACK_PAINT = "\033[30m";
const string RED_PAINT = "\033[31m";
const string GREEN_PAINT = "\033[32m";
const string YELLOW_PAINT = "\033[33m";
const string BLUE_PAINT = "\033[34m";
const string MAGENTA_PAINT = "\033[35m";
const string CYAN_PAINT = "\033[36m";
const string WHITE_PAINT = "\033[37m";

/*Menu*/
void menu();                   // creates the menu
void newGame();                // creates a new game for player
void leaderBoard();            // read player data from the player_data.txt file
void dataWriter(Profile);      // write data into player_data.txt file after eac game
void dataSort(Profile[], int); // sort player data for the leaderboard

/*Main Game*/
void gamePlay(int **&, int &, int, Profile);   // main game function: calls movement functions, pause options and etc...
void drawBoard(int **&, int &, int);           // prints the game board on the given sizes
void randomizer(int **&, int);                 // insert 2 or 4 into random locations
void scoreBoard(int **&, int &, int, Profile); // shows the final score of the game (called via gamePlay)
bool checkBoard(int **&, int);                 // checks win or lose conditions (called via gamePlay)
bool movePossibility(int **&, int **, int);    // if the board is changed it will add a random number, else it won't change anything

// front-end stuff
string randPhrase();                 // a fun bit that cheers up the player with random phrases
string randPaint(int, int, int **&); // chooses a random color for the board color

/*Movements*/
// UP
void upMove(int **&, int);       // move all numbers up if the top number is "0"
void upSum(int **&, int &, int); // sum and swap equal numbers in the up direction (also counts the score)

// DOWNs
void downMove(int **&, int);       // move all numbers down if the lower number is "0"
void downSum(int **&, int &, int); // sum and swap equal numbers in the down direction (also counts the score)

// RIGHT
void rightMove(int **&, int);       // move all numbers right if the right number is "0"
void rightSum(int **&, int &, int); // sum and swap equal numbers in the right direction (also counts the score)

// LEFT
void leftMove(int **&, int);       // move all numbers left if the left number is "0"
void leftSum(int **&, int &, int); // sum and swap equal numbers in the left direction (also counts the score)

/*Main*/
int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));
    system("cls");

    cout << YELLOW_PAINT << " ━   ━       ━ " << RED_PAINT << "TM" << endl;
    cout << YELLOW_PAINT << "  ┃ ┃ ┃ ┃ ┃ ┃ ┃" << endl;
    cout << " ━       ━   ━ " << endl;
    cout << "┃   ┃ ┃   ┃ ┃ ┃" << endl;
    cout << " ━   ━       ━ " << endl;
    cout << CYAN_PAINT << "by ~SrgtSajjad~" << RESET_PAINT << endl;
    cout << "Press any key to " << GREEN_PAINT << "START" << endl;
    char user_input = getch();
    menu();

    return 0;
}

/*Menu*/
void menu()
{

    system("cls");
    cout << BLUE_PAINT << "{Menu}\n"
         << RESET_PAINT << endl;
    cout << "[n]New Game\n(start a new game)\n"
         << endl;
    cout << "[l]LeaderBoard\n(check who's on top)\n"
         << endl;
    cout << "[e]Exit\n(close the game)" << endl;

    char user_input = getch();

    if (user_input == 'n')
    {
        newGame();
    }
    else if (user_input == 'l')
    {
        leaderBoard();
    }
    else if (user_input == 'e')
    {
        cout << endl;
        cout << "Do You Want To " << RED_PAINT << "Exit" << RESET_PAINT << "?\n[y]Yes / [n]No" << endl;
        char exit_sequence = getch();
        if (exit_sequence == 'y')
        {

            exit(0);
        }
        else
        {
            system("cls");
            menu();
        }
    }
    else
    {
        cout << "\u2573 Error-Please try again";
        Sleep(300);
        menu();
    }
}

void newGame()
{
    system("cls");
    string name;
    int size;
    Profile player;
    // make the player's profile
    cout << "Please enter your name: ";
    cin >> player.name;
    cout << endl;
    cout << "Board size: ";
    cin >> size;

    player.size = size;

    system("cls");

    // allocate board memory
    int **board = new int *[size];
    for (int i = 0; i < size; i++)
    {
        board[i] = new int[size];
    }

    // set 0 as first values of board
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            board[i][j] = 0;
        }
    }

    int score = 0;
    string paint = " ";

    // initiate the first two random numbers on the board
    randomizer(board, size);
    randomizer(board, size);

    gamePlay(board, score, size, player); // start game sequence
}

void leaderBoard()
{
    system("cls");

    int count = 0;
    char ch;
    string line;

    ifstream temp_file("player_data.txt");
    while (getline(temp_file, line)) // count number of data
    {
        count++;
    }
    temp_file.close(); // close to reset cursor

    Profile pl[count];
    ifstream file("player_data.txt"); // re-open file

    for (int i = 0; i < count; i++) // set up player data in Profile struct
    {
        string name = "", size = "", score = "";
        while (file.get(ch))
        {
            score += ch;
            if (ch == '.')
            {
                while (file.get(ch))
                {
                    size += ch;
                    if (ch == '.')
                    {
                        while (file.get(ch))
                        {
                            if (ch == '\n')
                            {
                                break;
                            }
                            name += ch;
                        }
                        break;
                    }
                }
                break;
            }
        }

        pl[i].name = name;
        pl[i].score = stoi(score);
        pl[i].size = stoi(size);
    }
    file.close();

    if (count > 0)
    {
        ////////////////////////////////
        dataSort(pl, count); // sort player data
        ////////////////////////////////

        cout << YELLOW_PAINT << "Rank / Name / Score / (Board Size)" << RESET_PAINT << endl;

        int num = 0;
        for (int i = count - 1; i >= count - 6; i--) // print top five
        {
            num++;
            if (num > count)
                break;
            cout << num << "." << pl[i].name << " " << pl[i].score << " (" << pl[i].size << ")" << endl;
        }
    }

    else
    {
        cout << "No data available yet!";
    }

    cout << endl;
    cout << "Press any key to leave to menu";
    getch();
    menu();
}

void dataWriter(Profile player)
{
    ofstream file("player_data.txt", ios::app);

    file << player.score << "." << player.size << "." << player.name << "\n";
    file.close();
}

void dataSort(Profile pl[], int count)
{
    for (int i = 0; i < count - 1; ++i)
    {

        for (int j = 0; j < count - i - 1; ++j)
        {

            if (pl[j].score > pl[j + 1].score)
            {

                Profile temp = pl[j];
                pl[j] = pl[j + 1];
                pl[j + 1] = temp;
            }
        }
    }
}

/*Main Game*/
void gamePlay(int **&board, int &score, int size, Profile player)
{

    // allocate tempBoard memory
    int **tempBoard = new int *[size];
    for (int i = 0; i < size; i++)
    {
        tempBoard[i] = new int[size];
    }

    char user_input = ' '; // just so it doesn't choose a random character
    drawBoard(board, score, size);
    bool leave = false;
    cout << "\n[l]Leave Game" << endl;
    while (!leave)
    {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                tempBoard[i][j] = board[i][j];
            }
        }

        user_input = getch();

        if (user_input == 'w')
        {
            upMove(board, size);
            upSum(board, score, size);
            upMove(board, size);
            if (movePossibility(board, tempBoard, size) == false)
                continue;
        }

        else if (user_input == 'a')
        {
            leftMove(board, size);
            leftSum(board, score, size);
            leftMove(board, size);
            if (movePossibility(board, tempBoard, size) == false)
                continue;
        }

        else if (user_input == 's')
        {
            downMove(board, size);
            downSum(board, score, size);
            downMove(board, size);
            if (movePossibility(board, tempBoard, size) == false)
                continue;
        }

        else if (user_input == 'd')
        {
            rightMove(board, size);
            rightSum(board, score, size);
            rightMove(board, size);
            if (movePossibility(board, tempBoard, size) == false)
                continue;
        }

        else if (user_input == 'l')
        {
            cout << "Are you sure yoou want to leave the game?" << endl;
            cout << "[y]Yes / [n]No" << endl;
            user_input = getch();
            if (user_input == 'y')
            {
                leave = true;
            }
            drawBoard(board, score, size);
            continue;
        }

        else
        {
            cout << "\u2573 Error-Please try again";
            Sleep(500);
            drawBoard(board, score, size);
            continue;
        }

        if (checkBoard(board, size) == true)
        {
            randomizer(board, size);
            if (checkBoard(board, size) == false)
                break;
        }

        else
            break;
        drawBoard(board, score, size);

        if (score > 100)
        {
            cout << randPhrase() << endl; // cheer up the player
        }
        cout << "\n[l]Leave Game" << endl;
    }

    scoreBoard(board, score, size, player);
}

void drawBoard(int **&board, int &score, int size)
{
    system("cls");

    cout << RESET_PAINT;
    for (int k = 0; k < size; k++)
    {
        for (int i = 0; i < size; i++)
        {
            cout << randPaint(i, k, board);
            cout << "\u2554\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2557";
            cout << " ";
        }

        cout << endl;

        for (int i = 0; i < size; i++)
        {
            cout << randPaint(i, k, board);
            cout << "\u2551        \u2551";
            cout << " ";
        }

        cout << endl;

        for (int i = 0; i < size; i++)
        {
            cout << randPaint(i, k, board);
            cout << "\u2560\u2561 ";

            int temp = board[k][i];
            int digits = 0;

            while (temp > 0)
            {
                temp /= 10;
                digits++;
            }

            if (board[k][i] == 0)
            {
                cout << "    ";
            }

            else if (digits == 1)
            {
                cout << " " << board[k][i] << "  ";
            }

            else if (digits == 2)
            {
                cout << " " << board[k][i] << " ";
            }

            else if (digits == 3)
            {
                cout << board[k][i] << " ";
            }

            else if (digits == 4)
            {
                cout << board[k][i];
            }

            cout << " \u255E\u2563 ";
        }

        cout << endl;

        for (int i = 0; i < size; i++)
        {
            cout << randPaint(i, k, board);
            cout << "\u2551        \u2551";
            cout << " ";
        }

        cout << endl;

        for (int i = 0; i < size; i++)
        {
            cout << randPaint(i, k, board);
            cout << "\u255A\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u255D";
            cout << " ";
        }

        cout << endl;
    }
    cout << RESET_PAINT;

    cout << "SCORE:" << score << endl;
}

void randomizer(int **&board, int size)
{
    int chance[4] = {2, 2, 2, 4};
    bool flag = false;
    int randNum = rand() % 4;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            // check if board is full
            if (board[i][j] == 0)
            {
                while (!flag)
                {
                    int randRow = rand() % size, randCol = rand() % size;
                    if (board[randRow][randCol] == 0)
                    {
                        board[randRow][randCol] = chance[randNum];
                        flag = true;
                    }
                }
            }
        }
    }
}

void scoreBoard(int **&board, int &score, int size, Profile player)
{
    drawBoard(board, score, size);
    bool win = false;

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            // check if player has won
            if (board[i][j] == 2048)
            {
                win = true;
            }
        }
    }

    if (win == true)
    {
        cout << "Congratulations! You have WON :D" << endl;
    }

    else
    {
        cout << "That's unfortunate. Better Luck Next Time!" << endl;
    }
    cout << "Your Score: " << score << "\n\n";
    player.score = score;
    cout << "Press any key to leave!";

    for (int i = 0; i < size; i++) // deallocate board memory
    {
        delete[] board[i];
    }

    delete[] board;
    getch();
    dataWriter(player);
    menu();
}

bool checkBoard(int **&board, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            // check if board is full
            if (board[i][j] == 0)
            {
                return true;
            }

            if (board[i][j] == 2048)
            {
                return false;
            }
        }
    }

    // check up or down move possibility
    for (int i = 0; i < size - 1; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (board[i][j] == board[i + 1][j])
            {
                return true;
            }
        }
    }

    // check left or right move possibility
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size - 1; j++)
        {
            if (board[i][j] == board[i][j + 1])
            {
                return true;
            }
        }
    }

    return false;
}

bool movePossibility(int **&board, int **tempBoard, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (tempBoard[i][j] != board[i][j])
            {
                return true;
            }
        }
    }

    return false;
}

// front-end stuff
string randPhrase()
{
    string phrase[4] = {"Nice Move!", "Good Job!", "WOW!", "Your Awesome!"};

    int randNum = rand() % 4;

    return phrase[randNum];
}

string randPaint(int i, int k, int **&board)
{
    string colors[6] = {RED_PAINT, BLUE_PAINT, CYAN_PAINT, YELLOW_PAINT, GREEN_PAINT, MAGENTA_PAINT};
    int randNum = rand() % 6;

    if (board[k][i] == 2)
        return WHITE_PAINT;
    else if (board[k][i] == 4)
        return YELLOW_PAINT;
    else if (board[k][i] == 8)
        return GREEN_PAINT;
    else if (board[k][i] == 16)
        return CYAN_PAINT;
    else if (board[k][i] == 32)
        return MAGENTA_PAINT;
    else if (board[k][i] == 64)
        return RED_PAINT;
    else if (board[k][i] == 128)
        return BLUE_PAINT;
    else if (board[k][i] == 256)
        return RED_PAINT;
    else if (board[k][i] == 512)
        return MAGENTA_PAINT;
    else if (board[k][i] == 1024)
        return CYAN_PAINT;
    else if (board[k][i] == 2048)
        return GREEN_PAINT;

    return RESET_PAINT;
}

/*Movements*/
// UP
void upMove(int **&board, int size)
{
    int count = 0;
    while (count < size - 1)
    {
        count++;
        for (int i = 1; i <= size - 1; i++)
        {
            for (int j = 0; j < size; j++)
            {
                if (board[i - 1][j] == 0)
                {
                    board[i - 1][j] = board[i][j];
                    board[i][j] = 0;
                }
            }
        }
    }
}
void upSum(int **&board, int &score, int size)
{
    for (int i = 1; i <= size - 1; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (board[i][j] == board[i - 1][j])
            {
                board[i - 1][j] *= 2;
                board[i][j] = 0;
                score += board[i - 1][j];
            }
        }
    }
}

// DOWN
void downMove(int **&board, int size)
{
    int count = 0;
    while (count < size - 1)
    {
        count++;
        for (int i = size - 2; i >= 0; i--)
        {
            for (int j = 0; j < size; j++)
            {
                if (board[i + 1][j] == 0)
                {
                    board[i + 1][j] = board[i][j];
                    board[i][j] = 0;
                }
            }
        }
    }
}
void downSum(int **&board, int &score, int size)
{
    for (int i = size - 2; i >= 0; i--)
    {
        for (int j = 0; j < size; j++)
        {
            if (board[i][j] == board[i + 1][j])
            {
                board[i + 1][j] *= 2;
                board[i][j] = 0;
                score += board[i + 1][j];
            }
        }
    }
}

// RIGHT
void rightMove(int **&board, int size)
{
    int count = 0;
    while (count < size - 1)
    {
        count++;
        for (int i = 0; i < size; i++)
        {
            for (int j = size - 2; j >= 0; j--)
            {
                if (board[i][j + 1] == 0)
                {
                    board[i][j + 1] = board[i][j];
                    board[i][j] = 0;
                }
            }
        }
    }
}
void rightSum(int **&board, int &score, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = size - 2; j >= 0; j--)
        {
            if (board[i][j + 1] == board[i][j])
            {
                board[i][j + 1] *= 2;
                board[i][j] = 0;
                score += board[i][j + 1];
            }
        }
    }
}

// LEFT
void leftMove(int **&board, int size)
{
    int count = 0;
    while (count < size - 1)
    {
        count++;
        for (int i = 0; i < size; i++)
        {
            for (int j = 1; j <= size - 1; j++)
            {
                if (board[i][j - 1] == 0)
                {
                    board[i][j - 1] = board[i][j];
                    board[i][j] = 0;
                }
            }
        }
    }
}
void leftSum(int **&board, int &score, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 1; j <= size - 1; j++)
        {
            if (board[i][j] == board[i][j - 1])
            {
                board[i][j - 1] *= 2;
                board[i][j] = 0;
                score += board[i][j - 1];
            }
        }
    }
}