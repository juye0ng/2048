#include <conio.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define VERSION 1206
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define SPACE 32
#define p 112
#define P 80
#define ESC 27

typedef struct Pos {
  struct Pos *this;
  int _xpos;
  int _ypos;
  int _arg;

  bool (*isNew)(const struct Pos *this);
  void (*setPos)(struct Pos *this, int x, int y);
  void (*genNumRandomly)(struct Pos *this, int size);
  int (*getX)(const struct Pos *this);
  int (*getY)(const struct Pos *this);
  int (*getValue)(const struct Pos *this);
} Pos;

typedef struct G2048 {
  struct G2048 *this;
  int _max;
  int _boardsize;
  int _maxboardsize;
  bool _isOnGame;
  bool _isFinGame;
  int _curr;
  Pos *_NUMBER;
  int BOARD[50][50];

  int (*getData)(const struct G2048 *this, char *value);
  void (*setData)(struct G2048 *this, int max, int boardsize);
  int (*getBoardData)(const struct G2048 *this, int x, int y);
  void (*setBoardData)(struct G2048 *this, int x, int y, int data);
  bool (*isOnGame)(const struct G2048 *this);
  void (*setOnGame)(struct G2048 *this, bool data);
  bool (*isFinGame)(const struct G2048 *this);
  void (*setFinGame)(struct G2048 *this, bool data);
  void (*printBoard)(const struct G2048 *this);
  void (*clearBoard)(struct G2048 *this);
  void (*updateScore)(struct G2048 *this);
  void (*FinishGame)(struct G2048 *this);
  void (*genNum)(struct G2048 *this);
  bool (*checkDirAvailable)(const struct G2048 *this, int x, int y, int dir);
  bool (*isPlayAvailable)(struct G2048 *this);
  bool (*hasNoSpace)(const struct G2048 *this);
  void (*applyDir)(struct G2048 *this, int _dir);
} G2048;

Pos *pos(int size);
void DeletePos(struct Pos *pos_ptr) { free(pos_ptr); }
bool isNew(const struct Pos *this, int x, int y);
void setPos(struct Pos *this, int x, int y);
void genNumRandomly(struct Pos *this, int size);
int getX(const struct Pos *this);
int getY(const struct Pos *this);
int getValue(const struct Pos *this);

G2048 *init();
void uninit(struct G2048 *G2048_ptr) { free(G2048_ptr); }
int getData(const struct G2048 *this, char *value);
void setData(struct G2048 *this, int max, int boardsize);
int getBoardData(const struct G2048 *this, int x, int y);
void setBoardData(struct G2048 *this, int x, int y, int data);
bool isOnGame(const struct G2048 *this);
void setOnGame(struct G2048 *this, bool data);
bool isFinGame(const struct G2048 *this);
void setFinGame(struct G2048 *this, bool data);
void printBoard(const struct G2048 *this);
void clearBoard(struct G2048 *this);
void updateScore(struct G2048 *this);
void FinishGame(struct G2048 *this);
void genNum(struct G2048 *this);
bool checkDirAvailable(const struct G2048 *this, int x, int y, int dir);
bool isPlayAvailable(struct G2048 *this);
bool hasNoSpace(const struct G2048 *this);
void applyDir(struct G2048 *this, int _dir);

void gotoxy(int x, int y);
void SETNOCURSOR();
void clearConsole();
void initialize();
void getInput();
void PAUSEGAME();
void EXITGAME();
void gameOver();
void _inGame();
void inGame();

G2048 *Game_main;
bool _isExit = false;
bool isKeyChanged = false;
int key;

Pos *pos(int size) {
  Pos *tmp = (Pos *)malloc(sizeof(Pos));
  tmp->this = tmp;
  tmp->_xpos = 0;
  tmp->_ypos = 0;
  tmp->_arg = 0;
  tmp->isNew = isNew;
  tmp->setPos = setPos;
  tmp->genNumRandomly = genNumRandomly;
  tmp->getX = getX;
  tmp->getY = getY;
  tmp->getValue = getValue;
  tmp->genNumRandomly(tmp, size);
  return tmp;
}

bool isNew(const struct Pos *this, int x, int y) {
  if (x == this->_xpos && y == this->_ypos)
    return true;
  else
    return false;
}

void setPos(struct Pos *this, int x, int y) {
  this->_xpos = x;
  this->_ypos = y;
}

void genNumRandomly(struct Pos *this, int size) {
  int num = (rand() % 10) + 1;
  this->_xpos = rand() % size;
  this->_ypos = rand() % size;
  if (num < 9)
    this->_arg = 2;
  else
    this->_arg = 4;
}

int getX(const struct Pos *this) { return this->_xpos; }

int getY(const struct Pos *this) { return this->_ypos; }

int getValue(const struct Pos *this) { return this->_arg; }

G2048 *init() {
  G2048 *tmp = (G2048 *)malloc(sizeof(G2048));
  tmp->this = tmp;
  tmp->getData = getData;
  tmp->setData = setData;
  tmp->getBoardData = getBoardData;
  tmp->setBoardData = setBoardData;
  tmp->isOnGame = isOnGame;
  tmp->setOnGame = setOnGame;
  tmp->isFinGame = isFinGame;
  tmp->setFinGame = setFinGame;
  tmp->printBoard = printBoard;
  tmp->clearBoard = clearBoard;
  tmp->updateScore = updateScore;
  tmp->FinishGame = FinishGame;
  tmp->genNum = genNum;
  tmp->checkDirAvailable = checkDirAvailable;
  tmp->isPlayAvailable = isPlayAvailable;
  tmp->hasNoSpace = hasNoSpace;
  tmp->applyDir = applyDir;
  tmp->_max = 0;
  tmp->_boardsize = 4;
  tmp->_maxboardsize = 11;
  tmp->_NUMBER = pos(tmp->_boardsize);
  return tmp;
}

int getData(const struct G2048 *this, char *value) {
  if (strcmp(value, "max"))
    return this->_max;
  else if (strcmp(value, "boardsize"))
    return this->_boardsize;
  else if (strcmp(value, "maxboardsize"))
    return this->_maxboardsize;
  else
    return 0;
}

void setData(struct G2048 *this, int max, int boardsize) {
  if (max != -1)
    this->_max = max;
  if (boardsize != -1)
    this->_boardsize = boardsize;
}

int getBoardData(const struct G2048 *this, int x, int y) {
  return this->BOARD[x][y];
}

void setBoardData(struct G2048 *this, int x, int y, int data) {
  this->BOARD[x][y] = data;
}

bool isOnGame(const struct G2048 *this) { return this->_isOnGame; }

void setOnGame(struct G2048 *this, bool data) { this->_isOnGame = data; }

bool isFinGame(const struct G2048 *this) { return this->_isFinGame; }

void setFinGame(struct G2048 *this, bool data) { this->_isFinGame = data; }

void printBoard(const struct G2048 *this) {
  clearConsole();
  printf("2048 with C ver %d \n", VERSION, this->_boardsize,
         this->_boardsize);
  printf("조작: 방향키, 신규생성숫자: *, 일시정지: P, 게임 종료: ESC\n");
  printf("현재 점수: %5d, 현재 최고 점수(숫자): ", this->_curr);
  if (this->_curr == this->_max)
    printf("*");
  printf("%5d\n", this->_max);
  for (int i = 0; i < this->_boardsize; i++) {
    for (int j = 0; j < this->_boardsize; j++) {
      if (i == this->_NUMBER->getX(this->_NUMBER->this) &&
          j == this->_NUMBER->getY(this->_NUMBER->this)) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
        printf("%5d  ", this->getBoardData(this, i, j));
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
      } else if (this->getBoardData(this, i, j) == 0) {
        printf("%5c  ", '*');
      } else
        printf("%5d  ", this->getBoardData(this, i, j));
    }
    printf("\n\n");
  }
}

void clearBoard(struct G2048 *this) {
  this->_isFinGame = false;
  this->_curr = 1;
  for (int i = 0; i < this->_boardsize; i++) {
    for (int j = 0; j < this->_boardsize; j++) {
      this->setBoardData(this, i, j, 0);
    }
  }
}

void updateScore(struct G2048 *this) {
  if (this->_curr > this->_max)
    this->_max = this->_curr;
}

void FinishGame(struct G2048 *this) {
  this->_isFinGame = true;
  this->_isOnGame = false;
}

void genNum(struct G2048 *this) {
  if (this->hasNoSpace(this) == true) {
    this->_NUMBER->setPos(this->_NUMBER->this, -1, -1);
    return;
  }
  while (1) {
    this->_NUMBER->genNumRandomly(this->_NUMBER->this, this->_boardsize);
    if (this->getBoardData(this, this->_NUMBER->getX(this->_NUMBER->this),
                           this->_NUMBER->getY(this->_NUMBER->this)) == 0) {
      this->setBoardData(this, this->_NUMBER->getX(this->_NUMBER->this),
                         this->_NUMBER->getY(this->_NUMBER->this),
                         this->_NUMBER->_arg);
      if (this->getBoardData(this, this->_NUMBER->getX(this->_NUMBER->this),
                             this->_NUMBER->getY(this->_NUMBER->this)) >
          this->_curr) {
        this->_curr =
            this->getBoardData(this, this->_NUMBER->getX(this->_NUMBER->this),
                               this->_NUMBER->getY(this->_NUMBER->this));
        this->updateScore(this);
      }
      return;
    }
  }
}

bool checkDirAvailable(const struct G2048 *this, int x, int y, int dir) {
  switch (dir) {
  case UP:
    if (x > 0)
      return true;
    else
      return false;
    break;
  case LEFT:
    if (y > 0)
      return true;
    else
      return false;
    break;
  case DOWN:
    if (x < this->_boardsize)
      return true;
    else
      return false;
    break;
  case RIGHT:
    if (y < this->_boardsize)
      return true;
    else
      return false;
    break;
  default:
    break;
  }
  return false;
}

bool isPlayAvailable(struct G2048 *this) {
  if (this->_isFinGame == true)
    return false;
  else if (this->hasNoSpace(this) == false)
    return true;
  for (int i = this->_boardsize; i > 0; i--) {
    for (int j = 0; j < this->_boardsize; j++) {
      if (this->getBoardData(this, i - 1, j) == 0) {
        this->setBoardData(this, i - 1, j, this->getBoardData(this, i, j));
        this->setBoardData(this, i, j, 0);
      }
    }
  }
  for (int i = 0; i < this->_boardsize - 1; i++) {
    for (int j = 0; j < this->_boardsize; j++) {
      if (this->getBoardData(this, i + 1, j) == 0) {
        this->setBoardData(this, i + 1, j, this->getBoardData(this, i, j));
        this->setBoardData(this, i, j, 0);
      }
    }
  }
  for (int j = this->_boardsize - 1; j > 0; j--) {
    for (int i = 0; i < this->_boardsize; i++) {
      if (this->getBoardData(this, i, j - 1) == 0) {
        this->setBoardData(this, i, j - 1, this->getBoardData(this, i, j));
        this->setBoardData(this, i, j, 0);
      }
    }
  }
  for (int j = 0; j < this->_boardsize - 1; j++) {
    for (int i = 0; i < this->_boardsize; i++) {
      if (this->getBoardData(this, i, j + 1) == 0) {
        this->setBoardData(this, i, j + 1, this->getBoardData(this, i, j));
        this->setBoardData(this, i, j, 0);
      }
    }
  }
  if (this->hasNoSpace(this) == true) {
    for (int i = 0; i < this->_boardsize; i++) {
      for (int j = 0; j < this->_boardsize; j++) {
        if (this->getBoardData(this, i, j) ==
                this->getBoardData(this, i + 1, j) ||
            this->getBoardData(this, i, j) ==
                this->getBoardData(this, i, j + 1) ||
            this->getBoardData(this, i, j) ==
                this->getBoardData(this, i - 1, j) ||
            this->getBoardData(this, i, j) ==
                this->getBoardData(this, i, j - 1))
          return true;
      }
    }
    this->FinishGame(this);
  } else
    return true;
}

bool hasNoSpace(const struct G2048 *this) {
  for (int i = 0; i < this->_boardsize; i++) {
    for (int j = 0; j < this->_boardsize; j++) {
      if (this->getBoardData(this, i, j) == 0)
        return false;
    }
  }
  return true;
}

void applyDir(struct G2048 *this, int _dir) {
  switch (_dir) {
  case UP:
    for (int i = 1; i < this->_boardsize; i++) {
      for (int j = 0; j < this->_boardsize; j++) {
        if (this->checkDirAvailable(this, i, j, _dir) == true) {
          for (int x = this->_boardsize - 1; x > 0; x--) {
            for (int y = 0; y < this->_boardsize; y++) {
              if (this->getBoardData(this, x - 1, y) == 0) {
                this->setBoardData(this, x - 1, y,
                                   this->getBoardData(this, x, y));
                this->setBoardData(this, x, y, 0);
              }
            }
          }
        }
        if (this->getBoardData(this, i, j) ==
            this->getBoardData(this, i - 1, j)) {
          this->setBoardData(this, i - 1, j,
                             this->getBoardData(this, i - 1, j) * 2);
          this->setBoardData(this, i, j, 0);
          if (this->getBoardData(this, i - 1, j) > this->_curr)
            this->_curr = this->getBoardData(this, i - 1, j);
        }
      }
    }
    break;
  case LEFT:
    for (int j = 1; j < this->_boardsize; j++) {
      for (int i = 0; i < this->_boardsize; i++) {
        if (this->checkDirAvailable(this, i, j, _dir) == true) {
          for (int y = this->_boardsize - 1; y > 0; y--) {
            for (int x = 0; x < this->_boardsize; x++) {
              if (this->getBoardData(this, x, y - 1) == 0) {
                this->setBoardData(this, x, y - 1,
                                   this->getBoardData(this, x, y));
                this->setBoardData(this, x, y, 0);
              }
            }
          }
        }
        if (this->getBoardData(this, i, j) ==
            this->getBoardData(this, i, j - 1)) {
          this->setBoardData(this, i, j - 1,
                             this->getBoardData(this, i, j - 1) * 2);
          this->setBoardData(this, i, j, 0);
          if (this->getBoardData(this, i, j - 1) > this->_curr)
            this->_curr = this->getBoardData(this, i, j - 1);
        }
      }
    }
    break;
  case DOWN:
    for (int i = this->_boardsize - 1; i > 0; i--) {
      for (int j = 0; j < this->_boardsize; j++) {
        if (this->checkDirAvailable(this, i, j, _dir) == true) {
          for (int x = 0; x < this->_boardsize - 1; x++) {
            for (int y = 0; y < this->_boardsize; y++) {
              if (this->getBoardData(this, x + 1, y) == 0) {
                this->setBoardData(this, x + 1, y,
                                   this->getBoardData(this, x, y));
                this->setBoardData(this, x, y, 0);
              }
            }
          }
        }
        if (this->getBoardData(this, i, j) ==
            this->getBoardData(this, i - 1, j)) {
          this->setBoardData(this, i, j,
                             this->getBoardData(this->this, i, j) * 2);
          this->setBoardData(this, i - 1, j, 0);
          if (this->getBoardData(this, i, j) > this->_curr)
            this->_curr = this->getBoardData(this, i, j);
        }
      }
    }
    break;
  case RIGHT:
    for (int j = this->_boardsize - 1; j > 0; j--) {
      for (int i = 0; i < this->_boardsize; i++) {
        if (this->checkDirAvailable(this, i, j, _dir) == true) {
          for (int y = 0; y < this->_boardsize - 1; y++) {
            for (int x = 0; x < this->_boardsize; x++) {
              if (this->getBoardData(this, x, y + 1) == 0) {
                this->setBoardData(this, x, y + 1,
                                   this->getBoardData(this, x, y));
                this->setBoardData(this, x, y, 0);
              }
            }
          }
        }
        if (this->getBoardData(this, i, j) ==
            this->getBoardData(this, i, j - 1)) {
          this->setBoardData(this, i, j, this->getBoardData(this, i, j) * 2);
          this->setBoardData(this, i, j - 1, 0);
          if (this->getBoardData(this, i, j) > this->_curr)
            this->_curr = this->getBoardData(this, i, j);
        }
      }
    }
    break;
  default:
    break;
  }
  this->updateScore(this);
}

void gotoxy(int x, int y) {
  COORD pos = {2 * x, y};
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void SETNOCURSOR() {
  CONSOLE_CURSOR_INFO CurInfo;
  CurInfo.dwSize = 1;
  CurInfo.bVisible = FALSE;
  SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}

void clearConsole() { system("cls"); }

void initialize() {
  char message[100][100] = {
      "□□□□□□□□□□□□□□□□□□□□□□□□□", "□□■■■□□□■■■□□□□□■□□□■■■□□",
      "□■□□□■□■□□□■□□□■■□□■□□□■□", "□■□□□■□■□□□■□□□■■□□■□□□■□",
      "□□□□□■□■□□□■□□■□■□□■□□□■□", "□□□□■□□■□□□■□□■□■□□□■■■□□",
      "□□□■□□□■□□□■□■□□■□□■□□□■□", "□□■□□□□■□□□■□■□□■□□■□□□■□",
      "□■□□□□□■□□□■□■■■■■□■□□□■□", "□■□□□□□■□□□■□□□□■□□■□□□■□",
      "□■■■■■□□■■■□□□□□■□□□■■■□□", "□□□□□□□□□□□□□□□□□□□□□□□□□"};
  clearConsole();
  for (int i = 0; i < 12; i++) {
    printf("%s\n", message[i]);
  }
  printf("\n");
  printf("PRESS ANY KEY TO START...");
  getch();
  clearConsole();
  int xPos = rand() % Game_main->_boardsize;
  int yPos = rand() % Game_main->_boardsize;
  Game_main->setBoardData(Game_main, xPos, yPos, 2);
}

void getInput() {
  key = 0;
  if (kbhit()) {
    key = getch();
    if (key == 224) {
      isKeyChanged = true;
      do {
        key = getch();
      } while (key == 224);
      switch (key) {
      case LEFT:
        Game_main->applyDir(Game_main, LEFT);
        break;
      case RIGHT:
        Game_main->applyDir(Game_main, RIGHT);
        break;
      case DOWN:
        Game_main->applyDir(Game_main, DOWN);
        break;
      case UP:
        Game_main->applyDir(Game_main, UP);
      }
    } else {
      switch (key) {
      case SPACE:
        break;
      case P:
      case p:
        PAUSEGAME();
        break;
      case ESC:
        EXITGAME();
      }
    }

    while (kbhit())
      getch();
  }
}

void PAUSEGAME() {
  int i, j;
  int x = 0;
  int y = 11;
  gotoxy(x, y);
  printf("PAUSED.. PRESS ANY KEY TO RESUME");
  getch();
  Game_main->printBoard(Game_main);
}

void EXITGAME() {
  int i, j;
  int x = 0;
  int y = 11;
  gotoxy(x, y);
  printf("THE PROGRAM WILL BE CLOSED. PRESS ANY KEY");
  gotoxy(x, y + 1);
  printf("PRESS ESC KEY TO RETURN..");
  int ckey = getch();
  if (ckey == ESC)
    Game_main->printBoard(Game_main);
  else {
    system("cls");
    printf("FINISHED");
    exit(0);
  }
}

void gameOver() {
  if (_isExit == false) {
    Game_main->printBoard(Game_main);
  }
  printf("GAME OVER...\n");
  printf("PRESS ANY KEY TO RESTART");
  getch();
  Game_main->_isFinGame = false;
  Game_main->_isOnGame = false;
  Game_main->clearBoard(Game_main);
  return;
}

void _inGame() {
  initialize();
  while (Game_main->_isFinGame == false) {
    Game_main->genNum(Game_main);
    if (Game_main->isPlayAvailable(Game_main) == false)
      break;
    Game_main->printBoard(Game_main);
    while (isKeyChanged == false)
      getInput();
    isKeyChanged = false;
    printf("\n");
  }
}

void inGame() {
  while (_isExit == false) {
    _inGame();
    gameOver();
  }
}

int main() {
  SETNOCURSOR();
  Game_main = init();
  Game_main->clearBoard(Game_main);
  inGame();
}
