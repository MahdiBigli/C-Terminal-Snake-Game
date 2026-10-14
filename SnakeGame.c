#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#define R 7
#define C 3 * R
#define MAXSCORE 40
#define HEART_LIFE_TIME 10
#define MAX_NUM_OF_RECORD_IN_LIST 10
#define BOLD "\x1B[1m"
#define UNDERLINE "\x1B[4m"
#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define GOLD "\x1B[38;2;255;215;0m"
#define SILVER "\x1B[38;2;230;232;235m"
#define BRONZE "\x1B[38;2;205;127;50m"
#define GRAY "\x1B[38;2;180;180;180m"
#define COLOREND "\x1B[0m"
// #define DEBUG

enum Directions
{
    Right = 2,
    Up = 1,
    Down = -1,
    Left = -2
};

struct Points
{
    int x;
    int y;
};

typedef struct
{
    char name[20];
    unsigned short int score;
    unsigned int time;
} Record;

Record player;
struct Points body[MAXSCORE + 2]; // +2 : Because it has head and another part of body at first.
struct Points head;
struct Points lastPart;
struct Points heart;
unsigned short int score = 0;

void welcome(void);
void Print(char map[R][C]);
void makeHeart(char map[R][C]);
void newBody(int score);
void addBodyToMap(char map[R][C], int score, bool lastPartCondition);
void moving(char map[R][C]);
void moveUp(void);
void moveLeft(void);
void moveRight(void);
void moveDown(void);
void getCommond(char map[R][C], char *fileName);
void playAgain(char map[R][C], char *fileName);
void showRecords(char *fileName, char map[R][C]);
int saveRecord(char *fileName, char *name, int score, int recordTime);
int sortingList(char *fileName);
void replaceHeart(char map[R][C]);

int main()
{
    char fileName[15] = "recordList.dat";
    char map[R][C];
    unsigned int recordedTime = 0;
    welcome();
    printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
    getCommond(map, fileName);
} // end of main function

void welcome(void)
{

    printf(BOLD "Welcome !!!" COLOREND "\nWe are happy to see you !!!\nWhat is " UNDERLINE "your name" COLOREND "?\n---> ");
    scanf("%s", player.name);
}

void makeHeart(char map[R][C])
{
    bool isPlaceOfHeartOk = false;
    srand(time(NULL));
    heart.x = rand() % R;
    heart.y = (rand() % C / 3) * 3;
    short int count = 0;
    for (int i = 0; i < score + 2 && !isPlaceOfHeartOk; i++)
    {
        if (heart.x == body[i].x && heart.y == body[i].y)
        {
            heart.x += 3 * i + 1;
            heart.x = heart.x % R;
            heart.y += 3 * i + 1;
            heart.y = (heart.y % C / 3) * 3;
            i = -1;
            count++;
        }
        if (count == 4)
        {
            replaceHeart(map);
            isPlaceOfHeartOk = true;
        }
    }
    for (int i = 0; i < R; i++)
    {
        int num = 0;
        for (int j = 0; j < C; j++)
        {
            if (num % 3 == 0)
            {
                if (heart.x == i && heart.y == j)
                    map[i][j] = 'X';
                else if (map[i][j] == 3)
                    map[i][j] = '.';
            }
            num++;
        }
    }
} // end of makeHeart function

void newBody(int score) // قبل اجرای تابع نوع حرکت , وقنی حرکت میخواد بکنه باید هر قسمت بدنش باید بره جای قسمت جلو تر
{
    for (int i = score + 1; i >= 0; i--)
    {
        body[i + 1].x = body[i].x;
        body[i + 1].y = body[i].y;
    }
} // end of newBody function

void addBodyToMap(char map[R][C], int score, bool lastPartCondition)
{
    for (int i = 1; i < score + 2; i++)
        map[body[i].x][body[i].y] = 'O';
    if (!lastPartCondition)
        map[lastPart.x][lastPart.y] = '.'; // lastPartCondition means that this location must be replaced with '.' eighter not.
}

void Print(char map[R][C])
{
    for (int i = 0; i < R; i++)
    {
        for (int j = 0; j < C; j++)
        {
            if (map[i][j] == 'X')
                printf(RED "%c" COLOREND, map[i][j]);
            else if (i == body[score + 1].x && j == body[score + 1].y)
                printf(BOLD "%c" COLOREND, map[i][j]);
            else if (map[i][j] == 'O')
                printf(BOLD GREEN "%c" COLOREND COLOREND, map[i][j]);
            else
                printf(BOLD "%c" COLOREND, map[i][j]);
        }
        printf("\n");
    }
} // end of Print function

void moving(char map[R][C])
{
    enum Directions last_d = Right;
    enum Directions this_d;
    bool be_eaten_heart = false;
    bool isCorrectInput = false;
    bool isLost = false;
    void (*opOfMove)(void);
    for (int i = 2; i < MAXSCORE + 2; i++)
    {
        body[i].x = -1;
        body[i].y = -1;
    }
    char input;
    int startTime = time(NULL);
    int time1 = time(NULL);
    printf("Notice! In this game, your choice must be one of these:");
    while (score != MAXSCORE && !isLost)
    {
        int time2 = time(NULL);
        bool lastPartCondition = false;
        if (be_eaten_heart || isCorrectInput)
            lastPartCondition = true;
        addBodyToMap(map, score, lastPartCondition);
        lastPartCondition = false;
        map[head.x][head.y] = '0';
        printf("\n       W)up\nA)left      S)right\n      Z)down\n");
        printf("\n----> score: %d\n", score * 10);
        if (time1 + HEART_LIFE_TIME <= time2 || be_eaten_heart)
        {
            makeHeart(map);
            time1 = time2;
            be_eaten_heart = false;
        }
        Print(map);
        printf("Your movement? -----> ");
        input = getch();
        switch (input)
        {
        case 'w':
            opOfMove = &moveUp;
            this_d = Up;
            isCorrectInput = true;
            break;
        case 'a':
            opOfMove = &moveLeft;
            this_d = Left;
            isCorrectInput = true;
            break;
        case 's':
            opOfMove = &moveRight;
            this_d = Right;
            isCorrectInput = true;
            break;
        case 'z':
            opOfMove = &moveDown;
            this_d = Down;
            isCorrectInput = true;
            break;
        }
        if (last_d != -this_d && isCorrectInput == true)
        {
            lastPart.x = body[score + 1].x;
            lastPart.y = body[score + 1].y;
            (*opOfMove)();
            if (head.x == heart.x && head.y == heart.y)
            {
                body[score + 2].x = lastPart.x;
                body[score + 2].y = lastPart.y;
                score++;
                be_eaten_heart = true;
            }
            newBody(score);
            for (int i = 1; i < score + 2; i++)
            {
                if (body[i].x == head.x && body[i].y == head.y)
                {
                    isLost = true;
                    map[head.x][head.y] = 'X';
                }
            }
            body[0].x = head.x;
            body[0].y = head.y;
            last_d = this_d;
            isCorrectInput = false;
        }
        printf("\n\n- - - - - - - - - - - - - - - -\n");
    }
    player.score = score;
    if (score == MAXSCORE)
    {
        if (time(NULL) - startTime > 60)
            printf("----- You are winner !!! -----\n~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~\nYour recorded time is: %dm and %ds\n", (time(NULL) - startTime) / 60, (time(NULL) - startTime) % 60);
        else
            printf("----- You are winner !!! -----\n~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~\nYour recorded time is: %ds\n", (time(NULL) - startTime) % 60);
        printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
    }
    else
    {
        printf("\n----> score: %d\n", score * 10);
        Print(map);
        printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - -");
        printf("\nx x x x x x ----- You are lost ! :((( ----- x x x x x x\n");
        printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
    }
    player.time = time(NULL) - startTime;
} // end of moving function

void moveUp(void)
{
    if (head.x != 0)
        head.x -= 1;
    else
        head.x = R - 1;
}
void moveLeft(void)
{
    if (head.y != 0)
        head.y -= 3;
    else
        head.y = C - 3;
}
void moveRight(void)
{
    if (head.y != C - 3)
        head.y += 3;
    else
        head.y = 0;
}

void moveDown(void)
{
    if (head.x != R - 1)
        head.x += 1;
    else
        head.x = 0;
}

void getCommond(char map[R][C], char *fileName)
{
    static short int turn = 1;
    short int choice;
    char firstChoice[11];
    (turn == 1) ? strcpy(firstChoice, "New Play") : strcpy(firstChoice, "Play Again");
    printf(BOLD "Select an option:\n" COLOREND);
    printf("    1) " GREEN "%s" COLOREND "\n    2) Show Records\n    3) " RED "Exit :(" COLOREND "\n---> ", firstChoice);
    scanf("%hd", &choice);
    switch (choice)
    {
    case 1:
        turn++;
        playAgain(map, fileName);
        break;
    case 2:
        showRecords(fileName, map);
        break;
    case 3:
        printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
        printf("Thank you for playing this game ! %c%c%c\n", 3, 3, 3);
        printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - -");
        break;
    default:
        printf(UNDERLINE "Invalid option selected !\n" COLOREND);
        break;
    }
}

void showRecords(char *fileName, char map[R][C])
{
    Record temp;
    FILE *fPtr = fopen(fileName, "rb");
    short int count = 1;
    printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - -\n" BOLD "Top Records" COLOREND ":\n");
    while (count <= MAX_NUM_OF_RECORD_IN_LIST && fread(&temp, sizeof(Record), 1, fPtr) == 1)
    {
        if (count == 1)
            printf("   " BOLD GOLD "%hd)" COLOREND COLOREND " Name: " BOLD GOLD "%-10s" COLOREND COLOREND " ---> Time: " GOLD "%us" COLOREND "\t---> Score: " GOLD "%hu" COLOREND "\n", count, temp.name, temp.time, temp.score * 10);
        else if (count == 2)
            printf("   " BOLD SILVER "%hd)" COLOREND COLOREND " Name: " BOLD SILVER "%-10s" COLOREND COLOREND " ---> Time: " SILVER "%us" COLOREND "\t---> Score: " SILVER "%hu" COLOREND "\n", count, temp.name, temp.time, temp.score * 10);
        else if (count == 3)
            printf("   " BOLD BRONZE "%hd)" COLOREND COLOREND " Name: " BOLD BRONZE "%-10s" COLOREND COLOREND " ---> Time: " BRONZE "%us" COLOREND "\t---> Score: " BRONZE "%hu" COLOREND "\n", count, temp.name, temp.time, temp.score * 10);
        else
            printf("   %hd) Name: " GRAY "%-10s" COLOREND " ---> Time: " GRAY "%us" COLOREND "\t---> Score: " GRAY "%hu" COLOREND "\n", count, temp.name, temp.time, temp.score * 10);
        count++;
    }
    printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
    fclose(fPtr);
    getCommond(map, fileName);
}

int saveRecord(char *fileName, char *name, int score, int recordTime)
{
    if (sortingList(fileName) == 0)
    {
        printf("Your record was saved !\n");
        printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
        return 0;
    }
    return 1; // means not save
}

void playAgain(char map[R][C], char *fileName)
{
    player.score = score = 0;
    srand(time(NULL));
    heart.x = rand() % R;
    heart.y = (rand() % C / 3) * 3;
    for (int i = 0; i < R; i++)
    {
        int num = 0;
        for (int j = 0; j < C; j++)
        {
            if (num % 3 == 0)
            {
                if (heart.x == i && heart.y == j)
                    map[i][j] = 'X';
                else
                    map[i][j] = '.';
            }
            else
                map[i][j] = ' ';
            num++;
        }
    }
    body[0].x = head.x = 0;
    body[0].y = head.y = 0;
    body[1].x = 0, body[1].y = -1;
    lastPart.x = 0, lastPart.y = -1;
    moving(map);
    saveRecord(fileName, player.name, player.score, player.time);
    getCommond(map, fileName);
}

int sortingList(char *fileName)
{
    short int count = 1;
    bool saved = false;
    Record temp;
    FILE *fPtr = fopen(fileName, "rb");
    FILE *newFPtr = fopen("newData.dat", "wb");
    while (fread(&temp, sizeof(Record), 1, fPtr) == 1 && count <= MAX_NUM_OF_RECORD_IN_LIST)
    {
        if (!saved && (temp.score < player.score || (temp.score == player.score && temp.time > player.time)))
        {
            fwrite(&player, sizeof(Record), 1, newFPtr);
            saved = true;
        }
        fwrite(&temp, sizeof(Record), 1, newFPtr);
        count++;
    }
    if (count == 1 || (fread(&temp, sizeof(Record), 1, fPtr) != 1 && count <= MAX_NUM_OF_RECORD_IN_LIST && !saved))
    {
        fwrite(&player, sizeof(Record), 1, newFPtr);
        saved = true;
    }
    fclose(fPtr);
    fclose(newFPtr);
    remove(fileName);
    rename("newData.dat", fileName);
    if (saved == true)
        return 0;
    return 1;
}

void replaceHeart(char map[R][C])
{
    bool placeOk = true;
    map[heart.x][heart.y] = '.';
    for (int i = 0; i < R; i++)
    {
        for (int j = 0; j < R; j += 3)
        {
            for (int k = 0; k <= score + 2 && placeOk; k++)
            {
                if (i == body[k].x && j == body[k].y)
                    placeOk = false;
            }
            if (placeOk)
            {
                heart.x = i;
                heart.y = j;
                return;
            }
            placeOk = true;
        }
    }
}

// Completed items:
//  برای ماندگاری قلب زمانسنج گذاشتم
// اگر زمان 10 ثانیه تمام بشود یا قلب خورده شود محل قرار گیری قلب تغییر می‌کند
// اگه مار از محیط زد بیرون از اون طرفش در میاد
// به ازای هر قلبی که می خوره یکی به طولش اضافه میشه
// اگر ورودی غلط باشه حرکتی صورت نمیگیره
// نمی‌تونه یهویی 180 درجه بچرخه
//  قلب نمی‌تواند در محل بدن مار پدید بیاید ---> I don't know why it doesn't work correct sometimes ...!?
// مار به بدن خودش خورد می‌سوزه
//   char d = 4; // dimound
//   char h = 3; // heart