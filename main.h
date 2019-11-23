#include "../../Library/loadpng.h"
#include "../../Library/process_image.h"
#include <math.h>
#include <SDL2/SDL_mixer.h>
#include <GL/glut.h>

#define WIDTH 640
#define HEIGHT 480
#define INTERVAL 25

#define MAX_X 24
#define MAX_Y 19
#define MAX_X_NO_PADDING 22
#define MAX_Y_NO_PADDING 17

#define PADDING_START_X 64.0f
#define PADDING_START_Y 64.0f
#define PADDING_FULL_X 704.0f
#define PADDING_FULL_Y 544.0f

#define RADIAN 57.2958f

#define GRAVITY 0.65f
#define MAX_VELOCITY 13.0f

#define MENU_BG_MAX_OFFSET 1280

#define PLAYER_SIZE 16

#define TILE_SIZE 32
#define TILE_SIZE_HALF 16
#define TILE_MAX 44

#define SWITCH_MAX 10
#define SWITCH_SIZE 20

#define BTN_LVL_START_X 153
#define BTN_LVL_START_Y 256
#define BTN_LVL_SIZE 40
#define BTN_LVL_SIZE_FULL 42
#define BTN_LVL_MAX 41 // 40 + 1 (Level start at 1)
#define BTN_LVL_MAX_REAL 40
#define BTN_LVL_MAX_PER_ROW 8

#define ENEMY_MAX 25

#define BULLET_FLY_CHASE_OFFSET 2.0f

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
    ~c_Player() {}
    float s_x, s_y, x, y, vx, vy, gx, gy, Alpha;
    int s_Gra, Gra, Drt, Stt, Is_Run, Is_Jump;
    Rect Rct, Rct_Dead;
    void Import(int x, int y, int Gra);
    void Reload();
    void Set_Rct_Dead();
};

c_Player Player;
Image Img_Player_Spawn[4][2][8], Img_Player_Run[4][2][6], Img_Player_Stand[4][2], Img_Player_Jump[4][2], Img_Player_Death;

class c_Switch {
  public:
    c_Switch();
    ~c_Switch() {}
    float x, y;
    int Drt, Is_Touch;
    Rect Hitbox;
    void Set(float x, float y, int Drt);
};

c_Switch Switch[SWITCH_MAX];
c_Switch *Ptr_Switch;
int Switch_Count;
float Switch_Angle[2];
float Switch_Hitbox_H[4] = {12.0f, 6.0f, 12.0f, 6.0f}, Switch_Hitbox_V[4] = {6.0f, 12.0f, 6.0f, 12.0f};
int Switch_List_Touch[SWITCH_MAX];
int Switch_Count_Touch;
Rect Rct_Switch;

class c_Enemy {
  public:
    c_Enemy(int x, int y);
    ~c_Enemy() {}
    float x, y, vx, vy;
    int Stt, Gra, Gra_Map, Drt, Move, Move_Max;
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
    void Draw();
    void Action();
};

Image Img_Enemy_Block;
int Enemy_Block_W = 32, Enemy_Block_H = 32;
float Enemy_Block_Hitbox_W = 22.0f, Enemy_Block_Hitbox_H = 22.0f;
float Enemy_Block_Velocity[2] = {-2.0f, 2.0f};

class c_Enemy_Block_Ver : public c_Enemy {
  public:
    c_Enemy_Block_Ver(int x, int y, int Drt, int Move, int Move_Max);
    ~c_Enemy_Block_Ver() {}
    void Draw();
    void Action();
};

class c_Enemy_Worm : public c_Enemy {
  public:
    c_Enemy_Worm(int x, int y, int Gra, int Drt, int Move, int Move_Max);
    ~c_Enemy_Worm() {}
    void Draw();
    void Action();
};

Image Img_Enemy_Worm[4][2][6];
int Enemy_Worm_W = 32, Enemy_Worm_H = 32;
float Enemy_Worm_Hitbox_W = 20.0f, Enemy_Worm_Hitbox_H = 20.0f;
float Enemy_Worm_Velocity[2] = {-2.0f, 2.0f};

class c_Enemy_Shooter : public c_Enemy {
  public:
    c_Enemy_Shooter(int x, int y, int Drt, int Move, int Move_Max);
    ~c_Enemy_Shooter() {}
    void Draw();
    void Action();
};

Image Img_Enemy_Shooter[2][3];
int Enemy_Shooter_W = 32, Enemy_Shooter_H = 32;
float Enemy_Shooter_Hitbox_W = 22.0f, Enemy_Shooter_Hitbox_H = 22.0f;

class c_Enemy_Fly : public c_Enemy {
  public:
    c_Enemy_Fly(int x, int y, int Move, int Move_Max);
    ~c_Enemy_Fly() {}
    int Stt_Fire;
    Rect Rct_Fire;
    virtual void Draw();
    virtual void Action();
};

Image Img_Enemy_Fly[2][4], Img_Enemy_Fly_Fire[21];
int Enemy_Fly_W = 32, Enemy_Fly_H = 32, Enemy_Fly_Fire_W = 64, Enemy_Fly_Fire_H = 64;
float Enemy_Fly_Hitbox_W = 22.0f, Enemy_Fly_Hitbox_H = 22.0f;

class c_Enemy_Fly_Chase : public c_Enemy_Fly {
  public:
    c_Enemy_Fly_Chase(int x, int y, int Move, int Move_Max);
    ~c_Enemy_Fly_Chase() {}
    void Draw();
    void Action();
};

Image Img_Enemy_Fly_Chase[2][4];

class c_Enemy_Crusher : public c_Enemy {
  public:
    c_Enemy_Crusher(int x, int y, int Gra, int Move, int Move_Max);
    ~c_Enemy_Crusher() {}
    void Draw();
    void Action();
};

Image Img_Enemy_Crusher[4][13];
float Enemy_Crusher_Offset_Hitbox[13];
float Enemy_Crusher_Offset_Hitbox_Base[6] = {34.0f, 42.0f, 50.0f, 58.0f, 68.0f, 78.0f};

class c_Bullet {
  public:
    c_Bullet(float x, float y, float vx, float vy);
    ~c_Bullet() {}
    int Is_Alive;
    float x, y, vx, vy;
    Rect Rct, Hitbox;
    void Collision();
    virtual void Draw() {}
    virtual void Action() {}
};

class c_Bullet_Shooter : public c_Bullet {
  public:
    c_Bullet_Shooter(float x, float y, float vx);
    ~c_Bullet_Shooter() {}
    void Draw();
    void Action();
};

Image Img_Bullet_Shooter;
int Bullet_Shooter_W = 8, Bullet_Shooter_H = 8;
float Bullet_Shooter_Hitbox_W = 12.0f, Bullet_Shooter_Hitbox_H = 12.0f;
float Bullet_Shooter_Velocity[2] = {-4.0f, 4.0f};
float Bullet_Shooter_Offset[2] = {-14.0f, 14.0f};

class c_Bullet_Fly : public c_Bullet {
  public:
    c_Bullet_Fly(float x, float y, float vx, float vy, float Angle);
    ~c_Bullet_Fly() {}
    int Is_Explode;
    float Angle, Alpha_Explode;
    virtual void Draw();
    virtual void Action();
};

Rect Rct_Bullet_Fly;
Image Img_Bullet_Fly, Img_Bullet_Fly_Explode;
int Bullet_Fly_W = 4, Bullet_Fly_H = 8, Bullet_Fly_Explode_W = 24, Bullet_Fly_Explode_H = 24;
float Bullet_Fly_Hitbox_W = 12.0f, Bullet_Fly_Hitbox_H = 12.0f;
float Bullet_Fly_Velocity = 6.0f, Bullet_Fly_Offset = 3.0f;

class c_Bullet_Fly_Chase : public c_Bullet_Fly {
  public:
    c_Bullet_Fly_Chase(float x, float y, float vx, float vy, float Angle);
    ~c_Bullet_Fly_Chase() {}
    void Draw();
    void Action();
};

Rect Rct_Bullet_Fly_Chase;
Image Img_Bullet_Fly_Chase;
int Bullet_Fly_Chase_W = 6, Bullet_Fly_Chase_H = 12;

int Menu_Stt, Menu_Time, Menu_Active, Menu_Choice, Menu_Form_Stt, Menu_Max_Lvl;
float Menu_Offset, Menu_Alpha, Menu_Alpha_Offset, Menu_Go_Alpha;
Rect Rct_Menu_Main, Rct_Menu_Btn[2], Rct_Menu_Lvl, Rct_Menu_Btn_Lvl;
int Game_Stt, Game_Time, Game_Level, Game_Import, Game_Init, Game_Dest_Gra;
float Game_Alpha;
Image Img_Game_Back;
Rect Rct_Game_Back;
Rect Rct_Egg, Rct_Egg_Pick;
int Game_Stt_Egg, Game_Stt_Egg_Pick;
int Enemy_Time;

Image Img_Num[10];
Image Img_Menu_BG, Img_Menu_Main, Img_Menu_Btn[2], Img_Menu_Lvl;
Image Img_Menu_Btn_Lvl_Act, Img_Menu_Btn_Lvl_Pas, Img_Menu_Btn_Lvl[BTN_LVL_MAX];
Image_Data Imgd_Tile[TILE_MAX];
Image Img_Game_BG, Img_Switch, Img_Egg[4][2], Img_Egg_Pick[4];
Rect *Ptr_Rct;
Pos Pos_Menu_Btn[] = {{190.0f, 144.0f}, {190.0f, 66.0f}};

float Tmp_Dis_X, Tmp_Dis_Y, Tmp_Dis, Tmp_Vx, Tmp_Vy, Tmp_Angle, Tmp_Angle2, Offset_Angle;
char Str[40];
int Map[MAX_Y][MAX_X], Map_Tile[MAX_Y][MAX_X];
int Max_X, Max_Y;
int i_vx, i_vy, x_Map, y_Map;

int Loop_Time[] = {1, 2, 0}, Loop_Stt[] = {1, 2, 3, 4, 5, 0};
int Loop_4[] = {1, 2, 3, 0};
int Loop_6[] = {1, 2, 3, 4, 5, 0};

int Gra_Next[] = {1, 2, 3, 0};
int Gra_Prev[] = {3, 0, 1, 2};
int Gra_Reverse[] = {2, 3, 0, 1};
Pos Gra_Offset[] = {{0, GRAVITY}, {GRAVITY, 0}, {0, -GRAVITY}, {-GRAVITY, 0}};
int Gra_Mapping[] = {0, 1, 0, 1};
int Tile_Mapping[] = {CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL,
                      CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL,
                      CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL,
                      CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL,
                      CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL, CL_TILE_WALL,
                      CL_TILE_TRAP, CL_TILE_TRAP, CL_TILE_TRAP, CL_TILE_TRAP};

int A, B, C;

Mix_Music *Music_BackGround = NULL;
Mix_Chunk *Sound_Switch = NULL;
Mix_Chunk *Sound_Touch_Switch = NULL;
Mix_Chunk *Sound_Dead = NULL;
Mix_Chunk *Sound_Pick_Egg = NULL;

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
void Create_Image_Game_Back();
void Load_Player();
void Load_Tile();
void Load_Egg();
void Load_Enemy();
void Load_Tile();
void Load_Player();
void Init_Level();
void Init_Sound();
void Go_Menu();
void Reload();
void Game_Display();
void Game_Keyboard(unsigned char key, int x, int y);
void Game_Special(int key, int x, int y);
void Game_Special_Up(int key, int x, int y);
float Abs(float x);
void Switch_Reload();

#include "cPlayer.cpp"
#include "cSwitch.cpp"
#include "cBullet.cpp"
#include "cEnemy.cpp"
#include "aFunc.cpp"
#include "sound.cpp"
#include "move.cpp"
#include "menu.cpp"
#include "game.cpp"
