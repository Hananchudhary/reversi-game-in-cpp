#include <iostream>
#include<fstream>
#include<Windows.h>
using namespace std;
const int ROWS = 8;
const int COLS = 8;
const int s = 9;
void setColor(int foreground, int background = 0) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int colorAttribute = (background << 4) | foreground; // Combine foreground and background
    SetConsoleTextAttribute(hConsole, colorAttribute);
}
void getRowColbyLeftClick(int& rpos, int& cpos)
{
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD Events;
    INPUT_RECORD InputRecord;
    SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT |
        ENABLE_EXTENDED_FLAGS);
    do
    {
        ReadConsoleInput(hInput, &InputRecord, 1, &Events);
        if (InputRecord.Event.MouseEvent.dwButtonState ==

            FROM_LEFT_1ST_BUTTON_PRESSED)

        {
            cpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
            rpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
            break;
        }
    } while (true);
}
void gotoRowCol(int rpos, int cpos)
{
    COORD scrn;
    HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
    scrn.X = rpos;
    scrn.Y = cpos;
    SetConsoleCursorPosition(hOuput, scrn);
}
void printempty(int h, int k, int color) { // print vacant boxes
    char ch = 219;
    for (int i = 0;i < 3;i++) {
        gotoRowCol(30 + (k * 7), (2 + i) + (h * 3));
        for (int j = 0;j < 7;j++) {
            if (j == 6 || i == 2) {
                setColor(0, 0);
            }
            else {
                setColor(color, 0);
            }
            cout << ch;
        }
        cout << endl;
    }
}
void printmark(int h, int k, char mark) { // print mark 
    char ch = 219;
    int color = 0;
    if (mark == 'x')
        color = 8;
    else
        color = 10;
    for (int i = 0;i < 3;i++) {
        gotoRowCol(30 + (k * 7), (2 + i) + (h * 3));
        for (int j = 0;j < 7;j++) {
            if (j == 6 || i == 2) {
                setColor(0, 0);
            }
            else if (j != 0 || j != 5) {
                setColor(color, 0);
            }
            else {
                setColor(255, 255);
            }
            cout << ch;
        }
        cout << endl;
    }
}
void printBoard(char board[][s]) { 
    for (int h = 0;h < ROWS;h++) {
        for (int k = 0;k < COLS;k++) {
            if (board[h][k] == 'x') {
                printmark(h, k, 'x');
            }
            else if (board[h][k] == 'o') {
                printmark(h, k, 'o');
            }
            else {
                printempty(h, k, 255);
            }
        }
    }
}
void clickLocation(int& x, int& y) { // gets click location starting from index 0
    int row, col,temp=0;
    getRowColbyLeftClick(row, col);
    for (int i = 0;i < ROWS;i++) {
        if (col >= (2 + (i * 3)) && col <= 3 + (i * 3)) {
            x = i;
            temp++;
        }
    }
    for (int i = 0;i < COLS;i++) {
        if (row >= (30 + (i * 7)) && row <= (35 + (7 * i))) {
            y = i;
            temp++;
        }
    }
    if (temp != 2) {
        x = 9;
        y = 9;
    }
    if (row >= 13 && row <= 21 && col >= 8 && col <= 10) {
        x = 21;
        y = 21;
    }
    else if (row >= 13 && row <= 21 && col >= 14 && col <= 16) {
        x = 22;
        y = 22;
    }
}
void gamestart(char board[][s], int& ct) {
    ifstream reader("start.txt");
    int i = 0, j = 0;
    char ch;
    int temp = 0;
    while (reader.get(ch)) {
        if (ch != '\n' && temp == 0) {
            board[i][j] = ch;
            j++;
        }
        if (ch == '\n' && temp == 0) {
            j = 0;
            i++;
        }
        if (ch == '-') {
            temp = 1;
        }
        else if (temp == 1) {
            ct = ch - 48;
        }
    }
    reader.close();
}
void boardinitialization(char board[][s], int& ct, char& temp) {
    ifstream reader("board.txt");
    int i = 0, j = 0;
    char ch;
    int temp1 = 0, temp2 = 0;
    while (reader.get(ch)) {
        if (ch != '\n' && temp1 == 0 && temp2 == 0) {
            board[i][j] = ch;
            j++;
        }
        if (ch == '\n' && temp1 == 0 && temp2 == 0) {
            j = 0;
            i++;
        }
        if (ch == '_') {
            temp1 = 1;
        }
        else if (temp1 == 1) {
            temp = ch;
        }
        if (ch == '-') {
            temp2 = 1;
        }
        else if (temp2 == 1) {
            ct = ch - 48;
        }
    }
    reader.close();
}
void boardfile(char board[][s], int i, char ch) {
    ofstream output("board.txt");
    for (int i = 0;i < ROWS;i++) {
        for (int j = 0;j < COLS;j++) {
            output << board[i][j];
        }
        output << endl;
    }
    output << ch;
    output << '-' << i;
    output.close();
}
void undofile(char board[][s], int i) {
    ofstream output("undo.txt");
    for (int i = 0;i < ROWS;i++) {
        for (int j = 0;j < COLS;j++) {
            output << board[i][j];
        }
        output << endl;
    }
    output << '-' << i;
    output.close();
}
void undo(char board[][s], int& ct) {
    ifstream reader("undo.txt");
    int i = 0, j = 0;
    char ch;
    int temp = 0;
    while (reader.get(ch)) {
        if (ch != '\n' && temp == 0) {
            board[i][j] = ch;
            j++;
        }
        if (ch == '\n' && temp == 0) {
            j = 0;
            i++;
        }
        if (ch == '-') {
            temp = 1;
        }
        else if (temp == 1) {
            ct = ch - 48;
        }
    }
    reader.close();
}
bool validrow(char board[][s], int x, int y, char mark, char mark2, bool check) {
    bool flag = false;
    for (int i = y - 1;i > 0;i--) {
        if (board[x][i] == mark2) {
            if (board[x][i - 1] == mark) {
                flag = true;
            }
        }
        else {
            break;
        }
    }
    for (int i = y + 1;i < COLS - 1;i++) {
        if (board[x][i] == mark2) {
            if (board[x][i + 1] == mark) {
                flag = true;
            }
        }
        else {
            break;
        }
    }
    if (flag)
        return true;
    return false;
}
bool validcol(char board[][s], int x, int y, char mark, char mark2, bool check) {
    bool flag = false;
    for (int i = x - 1;i > 0;i--) {
        if (board[i][y] == mark2) {
            if (board[i - 1][y] == mark) {
                flag = true;
            }
        }
        else {
            break;
        }
    }
    for (int i = x + 1;i < ROWS - 1;i++) {
        if (board[i][y] == mark2) {
            if (board[i + 1][y] == mark) {
                flag = true;
            }
        }
        else {
            break;
        }
    }
    if (flag)
        return true;
    return false;
}
bool validdiag(char board[][s], int x, int y, char mark, char mark2, bool check) {
    bool flag = false;
    for (int i = y + 1, j = x - 1;i < COLS - 1 && j>0;i++, j--) {
        if (board[j][i] == mark2) {
            if (board[j - 1][i + 1] == mark) {
                flag = true;
            }
        }
        else {
            break;
        }
    }
    for (int i = y - 1, j = x + 1;i > 0 && j < ROWS - 1;i--, j++) {
        if (board[j][i] == mark2) {
            if (board[j + 1][i - 1] == mark) {
                flag = true;
            }
        }
        else {
            break;
        }
    }
    if (flag)
        return true;
    return false;
}
bool validndiag(char board[][s], int x, int y, char mark, char mark2, bool check) {
    bool flag = false;
    for (int i = y + 1, j = x + 1;i < COLS - 1 && j < ROWS - 1;i++, j++) {
        if (board[j][i] == mark2) {
            if (board[j + 1][i + 1] == mark) {
                flag = true;
            }
        }
        else {
            break;
        }
    }
    for (int i = y - 1, j = x - 1;i > 0 && j > 0;i--, j--) {
        if (board[j][i] == mark2) {
            if (board[j - 1][i - 1] == mark) {
                flag = true;
            }
        }
        else {
            break;
        }
    }
    if (flag)
        return true;
    return false;
}
bool validinput(char board[][s], int x, int y, char mark, bool check) {
    char mark2;
    if (mark == 'x') {
        mark2 = 'o';
    }
    else {
        mark2 = 'x';
    }
    if (x >= ROWS || x < 0 || y >= COLS || y < 0) {
        return false;
    }
    if (board[x][y] == 'x' || board[x][y] == 'o') {
        return false;
    }
    if (validrow(board, x, y, mark, mark2, check) || validcol(board, x, y, mark, mark2, check) || validdiag(board, x, y, mark, mark2, check) || validndiag(board, x, y, mark, mark2, check)) {
        return true;
    }
    return false;
}
void turn(char board[][s], int x, int y, char mark) {
    board[x][y] = mark;
}
void fliprow(char board[][s], int x, int y, char mark, char mark2, int& count, bool flag) {
    for (int i = y - 1;i > 0;i--) {
        if (board[x][i] == mark2) {
            if (board[x][i - 1] == mark) {
                for (int j = i;j < y;j++) {
                    if (board[x][j] == mark2) {
                        if (flag) {
                            board[x][j] = mark;
                            printmark(x, j, mark);
                        }
                        else
                            count++;
                    }
                }
            }
        }
        else {
            break;
        }
    }
    for (int i = y + 1;i < COLS - 1;i++) {
        if (board[x][i] == mark2) {
            if (board[x][i + 1] == mark) {
                for (int j = i;j > y;j--) {
                    if (board[x][j] == mark2) {
                        if (flag) {
                            board[x][j] = mark;
                            printmark(x, j, mark);
                        }
                        else
                            count++;
                    }
                }
            }
        }
        else {
            break;
        }
    }
}
void flipcol(char board[][s], int x, int y, char mark, char mark2, int& count, bool flag) {
    for (int i = x - 1;i > 0;i--) {
        if (board[i][y] == mark2) {
            if (board[i - 1][y] == mark) {
                for (int j = i;j < x;j++) {
                    if (board[j][y] == mark2) {
                        if (flag) {
                            board[j][y] = mark;
                            printmark(j, y, mark);
                        }
                        else
                            count++;
                    }
                }
            }
        }
        else {
            break;
        }
    }
    for (int i = x + 1;i < ROWS - 1;i++) {
        if (board[i][y] == mark2) {
            if (board[i + 1][y] == mark) {
                for (int j = i;j > x;j--) {
                    if (board[j][y] == mark2) {
                        if (flag) {
                            board[j][y] = mark;
                            printmark(j, y, mark);
                        }
                        else
                            count++;
                    }
                }
            }
        }
        else {
            break;
        }
    }
}
void flipdiagonal(char board[][s], int x, int y, char mark, char mark2, int& count, bool flag) {
    for (int i = y + 1, j = x - 1;i < COLS - 1 && j > 0;i++, j--) {
        if (board[j][i] == mark2) {
            if (board[j - 1][i + 1] == mark) {
                for (int h = i, k = j;h > y && j < x;h--, k++) {
                    if (board[k][h] == mark2) {
                        if (flag) {
                            board[k][h] = mark;
                            printmark(k, h, mark);
                        }
                        else
                            count++;
                    }
                }
            }
        }
        else {
            break;
        }
    }
    for (int i = y - 1, j = x + 1;i > 0 && j < ROWS - 1;i--, j++) {
        if (board[j][i] == mark2) {
            if (board[j + 1][i - 1] == mark) {
                for (int h = i, k = j;h < y && j > x;h++, k--) {
                    if (board[k][h] == mark2) {
                        if (flag) {
                            board[k][h] = mark;
                            printmark(k, h, mark);
                        }
                        else
                            count++;
                    }
                }
            }
        }
        else {
            break;
        }
    }
}
void flipnondiag(char board[][s], int x, int y, char mark, char mark2, int& count, bool flag) {
    for (int i = y + 1, j = x + 1;i < COLS - 1 && j < ROWS - 1;i++, j++) {
        if (board[j][i] == mark2) {
            if (board[j + 1][i + 1] == mark) {
                for (int h = i, k = j;h > y && k > x;h--, k--) {
                    if (board[k][h] == mark2) {
                        if (flag) {
                            board[k][h] = mark;
                            printmark(k, h, mark);
                        }
                        else
                            count++;
                    }
                }
            }
        }
        else {
            break;
        }
    }
    for (int i = y - 1, j = x - 1;i > 0 && j > 0;i--, j--) {
        if (board[j][i] == mark2) {
            if (board[j - 1][i - 1] == mark) {
                for (int h = i, k = j;h < y && k < x;h++, k++) {
                    if (board[k][h] == mark2) {
                        if (flag) {
                            board[k][h] = mark;
                            printmark(k, h, mark);
                        }
                        else
                            count++;
                    }
                }
            }
        }
        else {
            break;
        }
    }
}
void flip(char board[][s], int x, int y, char mark, int& count, bool flag) {
    char mark2;
    if (mark == 'x') {
        mark2 = 'o';
    }
    else {
        mark2 = 'x';
    }
    count = 0;
    fliprow(board, x, y, mark, mark2, count, flag);
    flipcol(board, x, y, mark, mark2, count, flag);
    flipdiagonal(board, x, y, mark, mark2, count, flag);
    flipnondiag(board, x, y, mark, mark2, count, flag);
}
bool end(char board[][s], char mark) {
    bool flag = false;
    for (int i = 0;i < ROWS;i++) {
        for (int j = 0;j < COLS;j++) {
            if (validinput(board, i, j, mark, false)) {
                flag = true;
                printempty(i, j, 25);
            }
            else if (board[i][j] == '.') {
                printempty(i, j, 255);
            }
        }
    }
    if (flag)
        return false;
    return true;
}
int wincheck(char board[][s]) {
    int ct1 = 0, ct2 = 0;
    bool flag1 = true;
    for (int i = 0;i < ROWS;i++) {
        for (int j = 0;j < COLS;j++) {
            if (board[i][j] == 'x') {
                ct1++;
            }
            else if (board[i][j] == 'o') {
                ct2++;
            }
        }
    }
    if (flag1 && ct1 > ct2) {
        return 1;
    }
    else if (flag1 && ct1 < ct2) {
        return 2;
    }
    else if (flag1 && ct1 == ct2) {
        return -1;
    }
}
void AIturn(char board[][s], int& x, int& y, char mark) {
    int count = 0;
    int max = 0, tempx = 9, tempy = 9;
    for (int i = 0;i < ROWS;i++) {
        for (int j = 0;j < COLS;j++) {
            if (validinput(board, i, j, mark, true)) {
                flip(board, i, j, mark, count, false);
                if (count > max) {
                    max = count;
                    tempx = i;
                    tempy = j;
                }
            }
        }
    }
    x = tempx;
    y = tempy;
}
bool continuecheck() {
    ifstream reader1("board.txt");
    ifstream reader2("start.txt");
    char ch1, ch2;
    while (reader1.get(ch1) && reader2.get(ch2)) {
        if (ch1 != ch2 || ch1 != '_' || ch1 != ',') {
            reader1.close();
            reader2.close();
            return false;
        }
    }
    reader1.close();
    reader2.close();
    return true;
}
void printmenu() {
    char ch = 219;
    for (int i = 0;i < 5;i++) {
        gotoRowCol(35, i + 6);
        for (int j = 0;j < 50;j++) {
            setColor(255, 255);
            cout << ch;
        }
        cout << endl;
    }
    gotoRowCol(50, 8);
    setColor(0, 255);
    cout << "O N E   P L A Y E R";
    setColor(255, 255);
    for (int i = 0;i < 5;i++) {
        gotoRowCol(35, i + 15);
        for (int j = 0;j < 50;j++) {
            setColor(255, 255);
            cout << ch;
        }
        cout << endl;
    }
    gotoRowCol(50, 17);
    setColor(0, 255);
    cout << "T W O  P L A Y E R";
}
void printmenu2(bool flag, int temp, char ch1) {
    char ch = 219;
    for (int i = 0;i < 5;i++) {
        gotoRowCol(35, i + 6);
        for (int j = 0;j < 50;j++) {
            setColor(255, 255);
            cout << ch;
        }
        cout << endl;
    }
    gotoRowCol(50, 8);
    setColor(0, 255);
    cout << "N E W   G A M E";
    setColor(255, 255);
    if (!flag && temp == 1 && ch1 != ',') {
        for (int i = 0;i < 5;i++) {
            gotoRowCol(35, i + 15);
            for (int j = 0;j < 50;j++) {
                setColor(255, 255);
                cout << ch;
            }
            cout << endl;
        }
        gotoRowCol(50, 17);
        setColor(0, 255);
        cout << "C  O  N  T  I  N  U  E ";
    }
}
void printendmenu(int ct) {
    char ch = 219;
    for (int i = 0;i < 5;i++) {
        gotoRowCol(35, i + 2);
        for (int j = 0;j < 50;j++) {
            setColor(255, 255);
            cout << ch;
        }
        cout << endl;
    }
    gotoRowCol(48, 4);
    setColor(0, 255);
    if (ct == 1) {
        cout << "G R E Y           W I N";
    }
    else if (ct == 2) {
        cout << "G R E E N         W I N";
    }
    else {
        cout << "M A T C H       D R A W";
    }
    setColor(255, 255);
    for (int i = 0;i < 5;i++) {
        gotoRowCol(35, i + 10);
        for (int j = 0;j < 50;j++) {
            setColor(255, 255);
            cout << ch;
        }
        cout << endl;
    }
    gotoRowCol(53, 12);
    setColor(0, 255);
    cout << "N E W  G A M E";
    for (int i = 0;i < 5;i++) {
        gotoRowCol(35, i + 18);
        for (int j = 0;j < 50;j++) {
            setColor(255, 255);
            cout << ch;
        }
        cout << endl;
    }
    gotoRowCol(56, 20);
    setColor(0, 255);
    cout << "E X I T";
}
void printundomenu() {
    char ch = 219;
    for (int i = 0;i < 3;i++) {
        gotoRowCol(13, i + 8);
        for (int j = 0;j < 9;j++) {
            setColor(255, 255);
            cout << ch;
        }
    }
    gotoRowCol(14, 9);
    setColor(0, 255);
    cout << "U N D O";
    for (int i = 0;i < 3;i++) {
        gotoRowCol(13, i + 14);
        for (int j = 0;j < 9;j++) {
            setColor(255, 255);
            cout << ch;
        }
    }
    gotoRowCol(14, 15);
    setColor(0, 255);
    cout << "R E D O";
}
int main()
{
    while (true) {
        char board[s][s]{ '\0' };
        char ch = ',';
        int i = 1;
        int x = 0, y = 0, temp = 0;
        int win = 0;
        int count = 0;
        bool flag = continuecheck();
        boardinitialization(board, i, ch);
        undofile(board, i);
        printmenu();
        while (temp == 0) {
            getRowColbyLeftClick(x, y);
            if (x >= 35 && x <= 84 && y >= 6 && y <= 10) {
                setColor(0, 0);
                system("cls");
                temp = 1;
                printmenu2(flag, temp, ch);
            }
            else if (x >= 35 && x <= 84 && y >= 15 && y <= 19) {
                setColor(0, 0);
                system("cls");
                temp = 2;
                printmenu2(flag, temp, ch);
            }
        }
        while (true) {
            getRowColbyLeftClick(x, y);
            if (x >= 35 && x <= 84 && y >= 6 && y <= 10) {
                setColor(0, 0);
                system("cls");
                gamestart(board, i);
                printBoard(board);
                break;
            }
            else if (x >= 35 && x <= 84 && y >= 15 && y <= 19 && !flag && temp == 1 && ch != ',') {
                setColor(0, 0);
                system("cls");
                boardinitialization(board, i, ch);
                printBoard(board);
                break;
            }
        }
        printundomenu();
        if (temp == 2)
            ch = ',';
        else
            ch = '_';
        while (true) {
            if (i % 2 != 0) {
                if (end(board, 'x')) {
                    break;
                }
                clickLocation(x, y);
                if (!validinput(board, x, y, 'x', true)) {
                    if (x == 21 && y == 21) {
                        undo(board, i);
                        printBoard(board);
                    }
                    else if (x == 22 && y == 22) {
                        boardinitialization(board, i, ch);
                        printBoard(board);
                    }
                    continue;
                }
                undofile(board, i);
                turn(board, x, y, 'x');
                printmark(x, y, 'x');
                flip(board, x, y, 'x', count, true);
            }
            else {
                if (end(board, 'o')) {
                    break;
                }
                if (temp == 2) {
                    clickLocation(x, y);
                    if (!validinput(board, x, y, 'o', true)) {
                        if (x == 21 && y == 21) {
                            undo(board, i);
                            printBoard(board);
                        }
                        else if (x == 22 && y == 22) {
                            boardinitialization(board, i, ch);
                            printBoard(board);
                        }
                        continue;
                    }
                    undofile(board, i);
                }
                else {
                    AIturn(board, x, y, 'o');
                }
                turn(board, x, y, 'o');
                printmark(x, y, 'o');
                flip(board, x, y, 'o', count, true);
            }
            i++;
            boardfile(board, i, ch);
        }
        win = wincheck(board);
        gamestart(board, i);
        boardfile(board, i, ch);
        setColor(0, 255);
        printendmenu(win);
        while (true) {
            getRowColbyLeftClick(x, y);
            if (x >= 35 && x <= 84 && y >= 10 && y <= 14) {
                break;
            }
            else if (x >= 35 && x <= 84 && y >= 18 && y <= 22) {
                return 0;
            }
            else {
                continue;
            }
        }
        setColor(0, 0);
        system("cls");
    }
    return 0;
}