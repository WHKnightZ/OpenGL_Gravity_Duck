#include "../../Library/loadpng.h"
#include "../../Library/process_image.h"
#include <SDL2/SDL_mixer.h>
#include <GL/glut.h>

#define MAX_X 20
#define MAX_Y 15
#define WIDTH 640
#define HEIGHT 480
#define INTERVAL 25

#define MENU_BG_MAX_OFFSET 1280

#define PLAYER_SIZE 16

#define TILE_SIZE 32
#define TILE_SIZE_HALF 16
#define TILE_MAX 44

#define SWITCH_MAX 6
#define SWITCH_SIZE 20

#define BTN_LVL_START_X 153
#define BTN_LVL_START_Y 256
#define BTN_LVL_SIZE 40
#define BTN_LVL_SIZE_FULL 42
#define BTN_LVL_MAX 41 // 40 + 1 (Level start at 1)
#define BTN_LVL_MAX_REAL 40
#define BTN_LVL_MAX_PER_ROW 8

#define CL_TILE_DEST_FIRST 40

#define HITBOX_SWITCH 10.0f

#define ENEMY_MAX 10

int POS_X, POS_Y;

enum MENU_STATUS {
    MENU_STT_GO,
    MENU_STT_MAIN,
    MENU_STT_LVL

};

enum GAME_STATUS {
    GAME_STT_BEGIN,
    GAME_STT_SPAWN,
    GAME_STT_PLAY,
    GAME_STT_DEAD,
    GAME_STT_PICK,
    GAME_STT_WIN,
    GAME_STT_END
};

enum COLLISION_TILE {
    CL_TILE_WALL,
    CL_TILE_NOTHING,
    CL_TILE_TRAP,
    CL_TILE_DEST
};

struct Rect {
    float Left, Right, Bottom, Top;
};

struct Pos {
    float x, y;
};

struct Point {
    int x, y;
};

class c_Player {
  public:
    c_Player();
    ~c_Player();
    float s_x, s_y, x, y, vx, vy, gx, gy, Alpha;
    int s_Gra, Gra, Drt, Stt, Is_Run, Is_Jump;
    Rect Rct, Rct_Dead;
    void Import(int x, int y, int Gra);
    void Reload();
    void Set_Rct_Dead();
};

c_Player Player;
Image Img_Player_Spawn[4][2][8], Img_Player_Run[4][2][6], Img_Player_Stand[4][2], Img_Player_Jump[4][2], Img_Player_Death;

class c_Enemy {
  public:
    c_Enemy(int x, int y);
    ~c_Enemy();
    float x, y;
    int Stt, Drt, Move, Move_Max;
    Rect Rct, Hitbox;
    void Collision();
    virtual void Draw() {}
    virtual void Action() {}
};

c_Enemy *Enemy[ENEMY_MAX];
int Enemy_Count;

class c_Enemy_Block_Hon : public c_Enemy {
  public:
    c_Enemy_Block_Hon(int x, int y, int Drt, int Move, int Move_Max);
    ~c_Enemy_Block_Hon() {}
    float vx;
    void Draw();
    void Action();
};

Image Img_Enemy_Block;
int Enemy_Block_W = 32, Enemy_Block_H = 32;
float Enemy_Block_Hitbox_W = 22.0f, Enemy_Block_Hitbox_H = 22.0f;

class c_Enemy_Block_Ver : public c_Enemy {
  public:
    c_Enemy_Block_Ver(int x, int y, int Drt, int Move, int Move_Max);
    ~c_Enemy_Block_Ver() {}
    float vy;
    void Draw();
    void Action();
};

class c_Switch {
  public:
    c_Switch();
    ~c_Switch();
    float x, y;
    Rect Hitbox;
    int Is_Touch;
    void Set(float x, float y);
};

c_Switch Switch[SWITCH_MAX];
c_Switch *Ptr_Switch;
int Switch_Count;
float Switch_Angle;
int Switch_List_Touch[SWITCH_MAX];
int Switch_Count_Touch;
Rect Rct_Switch;

int Menu_Stt, Menu_Time, Menu_Active, Menu_Choice, Menu_Form_Stt, Menu_Max_Lvl;
float Menu_Offset, Menu_Alpha, Menu_Alpha_Offset, Menu_Go_Alpha;
Rect Rct_Menu_Main, Rct_Menu_Btn[2], Rct_Menu_Lvl, Rct_Menu_Btn_Lvl;
int Game_Stt, Game_Time, Game_Level, Game_Import, Game_Init, Game_Dest_Gra;
float Game_Alpha;
Rect Rct_Egg, Rct_Egg_Pick;
int Game_Stt_Egg, Game_Stt_Egg_Pick;

void (*Menu_Display_Func[3])();
void (*Menu_Process_Func[3])();
void (*Game_Display_Func[7])();
void (*Game_Process_Func[7])();
void (*Collision_Tile_Func[4])();

void Draw_Rect(Rect *Ptr_Rct);
void Player_Move_Left();
void Player_Move_Right();
void Player_Move_Up();
void Player_Move_Down();
void Collision_Tile();
void Load_Texture(Image *img, const char *path);
void Map_Texture(Image *img);
void Create_List_Draw_Tile();
void Load_Player();
void Load_Tile();
void Reload();
void Game_Display();
void Game_Keyboard(unsigned char key, int x, int y);
void Game_Special(int key, int x, int y);
void Game_Special_Up(int key, int x, int y);

Image Img_Num[10];
Image Img_Menu_BG, Img_Menu_Main, Img_Menu_Btn[2], Img_Menu_Lvl;
Image Img_Menu_Btn_Lvl_Act, Img_Menu_Btn_Lvl_Pas, Img_Menu_Btn_Lvl[BTN_LVL_MAX];
Image_Data Imgd_Tile[TILE_MAX];
Image Img_Game_BG, Img_Switch, Img_Egg[4][2], Img_Egg_Pick[4];
Rect Rct;
Pos Pos_Menu_Btn[] = {{190.0f, 144.0f}, {190.0f, 66.0f}};

char Str[40];
int Map[MAX_Y][MAX_X], Map_Tile[MAX_Y][MAX_X];
int Max_X, Max_Y;
int i_vx, i_vy, x_Map, y_Map;
int Loop_Time[] = {1, 2, 0}, Loop_Stt[] = {1, 2, 3, 4, 5, 0};
int Gra_Next[] = {1, 2, 3, 0};
int Gra_Reverse[] = {2, 3, 0, 1};
Pos Gra_Offset[] = {{0, 0.7f}, {0.7f, 0}, {0, -0.7f}, {-0.7f, 0}};
int Gra_Mapping[] = {0, 1, 0, 1};
int Tile_Mapping[] = {CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL,
                      CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL,
                      CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL,
                      CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL,
                      CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL,
                      CL_TILE_TRAP, CL_TILE_TRAP, CL_TILE_TRAP, CL_TILE_TRAP};

int A, B, C;

#include "sound.cpp"
#include "cPlayer.cpp"
#include "cEnemy.cpp"
#include "cSwitch.cpp"
#include "aFunc.cpp"
#include "move.cpp"
#include "menu.cpp"
#include "game.cpp"
