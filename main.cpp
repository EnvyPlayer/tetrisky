#include <QCoreApplication>
#include <QLocale>
#include <QTranslator>
#include <curses.h>
#include <iostream>
#include <random>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

using tetris = char[7][7];

tetris line = {"      ",
               "  #   ",
               "  #   ",
               "  #   ",
               "  #   ",
               "      "};

tetris s =    {"      ",
               "  #   ",
               "  ##  ",
               "   #  ",
               "      ",
               "      "};

tetris invs = {"      ",
               "   #  ",
               "  ##  ",
               "  #   ",
               "      ",
               "      "};

tetris l =    {"      ",
               "  #   ",
               "  #   ",
               "  ##  ",
               "      ",
               "      "};

tetris invl = {"      ",
               "      ",
               "   #  ",
               "   #  ",
               "  ##  ",
               "      "};

tetris t =    {"      ",
               "  #   ",
               "  ##  ",
               "  #   ",
               "      ",
               "      "};

tetris tbox = {"      ",
               "      ",
               "  ##  ",
               "  ##  ",
               "      ",
               "      "};

long initRandom() {
  auto tm = time(nullptr);
  return rand_r((unsigned int *)&tm);
}
void stdcerr(QString str) {
//    std::cerr << str.toStdString();
}

tetris *pickPiece(long rnd, int *curPiece) {
  *curPiece = rnd % 7;
  mvprintw(2, 0, "piece %d", *curPiece);
  tetris *val;
  switch (*curPiece) {
  case 0:
    val = &line;
    stdcerr ("\nline\n");
    break;
  case 1:
    val = &s;
    stdcerr ("\ns\n");
    break;
  case 2:
    val = &invs;
    stdcerr ("\nsinv\n");
    break;
  case 3:
    val = &l;
    stdcerr ("\nl\n");
    break;
  case 4:
    val = &invl;
    stdcerr ("\ninvl\n");
    break;
  case 5:
    val = &t;
    stdcerr ("\nt\n");
    break;
  case 6:
    val = &tbox;
    stdcerr ("\ntbox\n");
    break;
  }
  return val;
}

void printTetris(int top, int left, int orientation, tetris *piece) {
  for (int i = 0; i < 36; i++) {
    if (orientation == -1 || !(top + (i / 6) >= 44 || left + (i % 6) <= 30 || left + (i % 6) >= 50))
        mvprintw(top + (i / 6), left + (i % 6), " ");
    char x[2] = " ";
    if (piece && (x[0] = (*piece)[i / 6][i % 6]) == '#')
        if(strncmp(x, "#", 1) == 0)
            mvprintw(top + (i / 6), left + (i % 6), "%c", x[0]);
  }
}

void clearTetris(int top, int left, int orientation, tetris *piece) {
  for (int i = 0; i < top ; i++ ) {
    mvprintw(4 + i, 30, "|                   |");
  }
  for (int i = 0; i < 36; i++) {
    char x[2] = " ";
    if (piece && (x[0] = (*piece)[i / 6][i % 6]) == '#')
      if(strncmp(x, "#", 1) == 0)
        mvprintw(top + (i / 6), left + (i % 6), "%s", x);
  }
}

void rotatePiece(bool clockwise, int orientation, tetris* piece) {
    tetris * temp = (tetris *) malloc(sizeof(char) * 49);
    if(clockwise) {
        for(int i = 0; i < 6; i++) {
            for (int j = 0; j < 6; j++) {
                (*temp)[i][6 - j] = (*piece)[i][j];
            }
        }
        for(int i = 0; i < 6; i++) {
            for (int j = 0; j < 6; j++) {
                (*piece)[i][j] = (*temp)[i][j];
            }
        }
    } else {
        for(int i = 0; i < 6; i++) {
            for (int j = 0; j < 6; j++) {
                (*temp)[i][j] = (*piece)[j][6 - i];
            }
        }
        for(int i = 0; i < 6; i++) {
            for (int j = 0; j < 6; j++) {
                (*piece)[i][j] = (*temp)[i][j];
            }
        }
    }
    free(temp);
}

enum COLLISIONCODE {
    C_NONE = 0,
    C_BOTTOM = 1,
    C_LEFT = 2,
    C_RIGHT = 4,
};

enum KEY {
    K_UP = 65,
    K_DOWN = 66,
    K_LEFT = 68,
    K_RIGHT = 67,
    K_DEL = 51
};

const int play_height = 40;

int collisionDetect(int top, int left, int orientation, tetris *piece) {
    tetris *subject = (tetris *)piece;

    if(left <= 28) return C_LEFT;
    for (int i = 0; i < 6; i++) {
        if(*subject[i][left - 28] == '#') {
            return C_LEFT;
        }
    }
    if(left >= 47) return C_RIGHT;
    for (int i = 0; i < 6; i++) {
        if(*subject[i][left - 40] == '#') {
            return C_RIGHT;
        }
    }

    if(top == play_height) return C_BOTTOM;


    return C_NONE;
}

void spawnNewTetris(tetris *nextTetris) { printTetris(4, 37, -1, nextTetris); }

void dropItBigger() {}

int main(int argc, char *argv[]) {
  QCoreApplication a(argc, argv);

  QTranslator translator;
  const QStringList uiLanguages = QLocale::system().uiLanguages();
  for (const QString &locale : uiLanguages) {
    const QString baseName = "tetrisky-qt_" + QLocale(locale).name();
    if (translator.load(":/i18n/" + baseName)) {
      a.installTranslator(&translator);
      break;
    }
  }

  initscr();
  printw("Welcome to Tetrisky, comrade!");
  mvprintw(3, 30, "---------------------");
  for (int i = 0; i < 40; i++) {
    mvprintw(4 + i, 30, "|                   |");
  }
  mvprintw(44, 30, "---------------------");
  mvprintw(10, 54, "--------");
  for (int i = 0; i < 6; i++) {
    mvprintw(11 + i, 54, "|      |");
  }
  mvprintw(17, 54, "--------");

  int step = 5;
  int column = 34;
  int stepFrame = 0;
  int orientation = 0;
  tetris *curPiece;
  int curPieceIdx = -1;
  const int FRAMES_PER_STEP = 25;

  set_escdelay(5);
  while (true) {
    timeout(20);
    long rnd = initRandom();
    tetris *nextPiece = pickPiece(rnd, &curPieceIdx);
    //mvprintw(24, 80, "debugger: %d", curPieceIdx);

    if(!curPiece) {
        spawnNewTetris(nextPiece);
        curPiece = nextPiece;
    }

    // clearTetris(11, 55, 0, )
    printTetris(11, 55, 0, nextPiece);

    int key = getch();
    switch (key) {
    case K_LEFT:
      column--;
      break;
    case K_RIGHT:
      column++;
      break;
    case K_UP:
      orientation += (orientation == 3 ? -3 : 1);
      break;
    case K_DOWN:
      orientation -= (orientation == 0 ? -3 : 1);
      break;
    case K_DEL:
      dropItBigger();
      break;
    }

    //mvprintw(24, 80, "debugger: %d", key);


    switch(collisionDetect(step, column, orientation, curPiece)) {
    case C_NONE:
      if (stepFrame++ > FRAMES_PER_STEP) {
        stepFrame = 0;
        clearTetris(step, column, orientation, curPiece);
        if(key == K_UP || key == K_DOWN)
            rotatePiece(key == K_DOWN, orientation, curPiece);
        step++;
        stdcerr (QString (" - %1 - %2 - %3 - %4").arg(step).arg(column).arg(orientation).arg((long)curPiece));
        printTetris(step, column, orientation, curPiece);
      } else {
        if(key == K_UP || key == K_DOWN)
            rotatePiece(key == K_DOWN, orientation, curPiece);
        stepFrame++;
      }
    break;
    case C_BOTTOM:
      spawnNewTetris(nextPiece);
      curPiece = nextPiece;
      step = 5;
      column = 34;
      break;
    case C_LEFT:
      column ++;
      break;
    case C_RIGHT:
      column --;
      break;
    }
  }
  refresh();
  getch();
  endwin();

  return a.exec();
}
