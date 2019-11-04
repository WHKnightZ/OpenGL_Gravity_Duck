#include "main.h"

void Load_Texture(Image *img, const char *path) {
    loadPngSwap(&img->img, &img->w, &img->h, path);
}

void Map_Texture(Image *img) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->w, img->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->img);
}

void Init_Level() {
    FILE *f;
    int Level = 1, Pos_X, Pos_Y;
    int Width, a, b, x, y = 16;
    while (true) {
        sprintf(Str, "Maps/%02d.txt", Level);
        f = fopen(Str, "r");
        if (f == NULL)
            break;
        Pos_X = BTN_LVL_START_X + ((Level - 1) % BTN_LVL_MAX_PER_ROW) * BTN_LVL_SIZE_FULL;
        Pos_Y = BTN_LVL_START_Y - ((Level - 1) / BTN_LVL_MAX_PER_ROW) * BTN_LVL_SIZE_FULL;
        Mix_Image(&Img_Menu_Lvl, &Img_Menu_Btn_Lvl_Pas, Pos_X, Pos_Y);
        if (Level >= 10) {
            a = Level / 10;
            b = Level % 10;
            Width = Img_Num[a].w + Img_Num[b].w + 2;
            x = 20 - Width / 2;
            Clone_Image(&Img_Menu_Btn_Lvl_Act, &Img_Menu_Btn_Lvl[Level]);
            Mix_Image(&Img_Menu_Btn_Lvl[Level], &Img_Num[a], x, y);
            Mix_Image(&Img_Menu_Lvl, &Img_Num[a], Pos_X + x, Pos_Y + y);
            x += Img_Num[a].w + 2;
            Mix_Image(&Img_Menu_Btn_Lvl[Level], &Img_Num[b], x, y);
            Mix_Image(&Img_Menu_Lvl, &Img_Num[b], Pos_X + x, Pos_Y + y);
        } else {
            a = Level % 10;
            x = 20 - Img_Num[a].w / 2;
            Clone_Image(&Img_Menu_Btn_Lvl_Act, &Img_Menu_Btn_Lvl[Level]);
            Mix_Image(&Img_Menu_Btn_Lvl[Level], &Img_Num[a], x, y);
            Mix_Image(&Img_Menu_Lvl, &Img_Num[a], Pos_X + x, Pos_Y + y);
        }
        fclose(f);
        Level++;
    }
    Menu_Max_Lvl = Level - 1;
    Set_Rect_Btn_Lvl();
    free(Img_Menu_Btn_Lvl_Act.img);
    free(Img_Menu_Btn_Lvl_Pas.img);
    for (int i = 0; i < 10; i++)
        free(Img_Num[i].img);
}

void Init_Game() {
    // GL
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, WIDTH, 0, HEIGHT);
    glViewport(0, 0, WIDTH, HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glClearColor(0.322f, 0.188f, 0.129f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // Game
    Init_Array_Func();
    for (int i = 0; i < 10; i++) {
        sprintf(Str, "Images/Nums/%d.png", i);
        Load_Texture(&Img_Num[i], Str);
    }
    Load_Texture(&Img_Menu_BG, "Images/Menu_BG.png");
    Load_Texture(&Img_Menu_Main, "Images/Menu_Main.png");
    Load_Texture(&Img_Menu_Btn[0], "Images/Btn_Start_Game.png");
    Load_Texture(&Img_Menu_Btn[1], "Images/Btn_More_Game.png");
    Load_Texture(&Img_Menu_Lvl, "Images/Menu_Lvl.png");
    Load_Texture(&Img_Menu_Btn_Lvl_Act, "Images/Btn_Lvl_Act.png");
    Load_Texture(&Img_Menu_Btn_Lvl_Pas, "Images/Btn_Lvl_Pas.png");
    Load_Texture(&Img_Game_BG, "Images/Game_BG.png");
    Load_Texture(&Img_Switch, "Images/Switch.png");
    Load_Texture(&Img_Enemy_Block, "Images/Block.png");
    Rct_Menu_Main.Left = (WIDTH - Img_Menu_Main.w) / 2;
    Rct_Menu_Main.Bottom = (HEIGHT - Img_Menu_Main.h) / 2;
    Rct_Menu_Main.Right = Rct_Menu_Main.Left + Img_Menu_Main.w;
    Rct_Menu_Main.Top = Rct_Menu_Main.Bottom + Img_Menu_Main.h;
    Rct_Menu_Lvl.Left = (WIDTH - Img_Menu_Lvl.w) / 2;
    Rct_Menu_Lvl.Bottom = (HEIGHT - Img_Menu_Lvl.h) / 2;
    Rct_Menu_Lvl.Right = Rct_Menu_Lvl.Left + Img_Menu_Lvl.w;
    Rct_Menu_Lvl.Top = Rct_Menu_Lvl.Bottom + Img_Menu_Lvl.h;
    for (int i = 0; i < 2; i++) {
        Rct_Menu_Btn[i].Left = Pos_Menu_Btn[i].x;
        Rct_Menu_Btn[i].Right = Rct_Menu_Btn[i].Left + Img_Menu_Btn[i].w;
        Rct_Menu_Btn[i].Bottom = Pos_Menu_Btn[i].y;
        Rct_Menu_Btn[i].Top = Rct_Menu_Btn[i].Bottom + Img_Menu_Btn[i].h;
    }
    Rct_Menu_Btn_Lvl.Left = 100;
    Rct_Menu_Btn_Lvl.Right = 140;
    Rct_Menu_Btn_Lvl.Bottom = 100;
    Rct_Menu_Btn_Lvl.Top = 140;
    Rct_Switch.Bottom = Rct_Switch.Left = -SWITCH_SIZE;
    Rct_Switch.Top = Rct_Switch.Right = SWITCH_SIZE;
    Load_Tile();
    Load_Player();
    Menu_Choice = 0;
    Init_Level();
    Menu_Alpha = 1.0f;
    Menu_Alpha_Offset = -0.03f;
    Menu_Stt = 0;
    Menu_Time = 0;
    Menu_Form_Stt = 0;
    Menu_Active = 1;
    Menu_Offset = MENU_BG_MAX_OFFSET;
    Game_Init = 0;
    Game_Import = 1;
}

void Timer(int value) {
    if (Menu_Active) {
        Menu_Alpha += Menu_Alpha_Offset;
        Menu_Stt++;
        if (Menu_Stt == 15) {
            Menu_Stt = 0;
            Menu_Alpha_Offset = -Menu_Alpha_Offset;
        }
        Menu_Offset -= 1.0f;
        if (Menu_Offset <= 0.0f)
            Menu_Offset += MENU_BG_MAX_OFFSET;
    } else {
        Game_Process_Func[Game_Stt]();
        Switch_Action();
        for (int i=0;i<Enemy_Count;i++)
    		Enemy[i]->Action();
    }
    glutPostRedisplay();
    glutTimerFunc(INTERVAL, Timer, 0);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    POS_X = (glutGet(GLUT_SCREEN_WIDTH) - WIDTH) >> 1;
    POS_Y = (glutGet(GLUT_SCREEN_HEIGHT) - HEIGHT) >> 1;
    glutInitWindowPosition(POS_X, POS_Y);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Gravity Duck");
    Init_Game();
    glutTimerFunc(0, Timer, 0);
    glutDisplayFunc(Menu_Display);
    glutKeyboardFunc(Menu_Keyboard);
    glutSpecialFunc(Menu_Special);
    glutMainLoop();
    return 0;
}
