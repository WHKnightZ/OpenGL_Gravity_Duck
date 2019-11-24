void Load_Texture(Image *img, const char *path) {
    Load_Png_Swap(&img->img, &img->w, &img->h, path);
}

void Map_Texture(Image *img) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->w, img->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->img);
}

void Reload_Matrix_Menu() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIDTH, 0, HEIGHT);
    glViewport(0, 0, WIDTH, HEIGHT);
    glMatrixMode(GL_MODELVIEW);
}

void Reload_Matrix_Game() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIDTH, 0, HEIGHT);
    glViewport(0, 0, WIDTH, HEIGHT);
    glTranslatef(-64.0f, -64.0f, 0.0f);
    glMatrixMode(GL_MODELVIEW);
}

void Load_Num() {
    int Pos[][4] = {
        {0, 0, 8, 8},
        {8, 0, 3, 8},
        {11, 0, 8, 8},
        {19, 0, 8, 8},
        {27, 0, 8, 8},
        {35, 0, 8, 8},
        {43, 0, 8, 8},
        {51, 0, 8, 8},
        {59, 0, 8, 8},
        {67, 0, 8, 8}};
    int *Ptr_Pos;
    Image Img, Img_Tmp;
    Load_Png(&Img.img, &Img.w, &Img.h, "Images/Menu/Nums.png");
    for (int i = 0; i < 10; i++) {
        Ptr_Pos = &Pos[i][0];
        Crop_Image(&Img, &Img_Tmp, *Ptr_Pos, *(Ptr_Pos + 1), *(Ptr_Pos + 2), *(Ptr_Pos + 3));
        Swap_Image(Img_Tmp.img, Img_Tmp.w, Img_Tmp.h);
        Img_Num[i] = Img_Tmp;
    }
    free(Img.img);
}

void Set_Rect_Btn_Lvl() {
    Rct_Menu_Btn_Lvl.Left = BTN_LVL_START_X + (Menu_Choice - 1) % BTN_LVL_MAX_PER_ROW * BTN_LVL_SIZE_FULL;
    Rct_Menu_Btn_Lvl.Bottom = BTN_LVL_START_Y - (Menu_Choice - 1) / BTN_LVL_MAX_PER_ROW * BTN_LVL_SIZE_FULL;
    Rct_Menu_Btn_Lvl.Right = Rct_Menu_Btn_Lvl.Left + BTN_LVL_SIZE;
    Rct_Menu_Btn_Lvl.Top = Rct_Menu_Btn_Lvl.Bottom + BTN_LVL_SIZE;
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
        Pos_X = BTN_LVL_START_INIT_X + ((Level - 1) % BTN_LVL_MAX_PER_ROW) * BTN_LVL_SIZE_FULL;
        Pos_Y = BTN_LVL_START_INIT_Y - ((Level - 1) / BTN_LVL_MAX_PER_ROW) * BTN_LVL_SIZE_FULL;
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
    free(Img_Menu_Btn_Lvl_Act.img);
    free(Img_Menu_Btn_Lvl_Pas.img);
    for (int i = 0; i < 10; i++)
        free(Img_Num[i].img);
}

void Init_Game() {
    // GL
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
    Load_Texture(&Img_Menu_BG, "Images/Menu/BG.png");
    Load_Texture(&Img_Menu_Main, "Images/Menu/Main.png");
    Load_Texture(&Img_Menu_Btn[0], "Images/Menu/Btn_Start_Game.png");
    Load_Texture(&Img_Menu_Btn[1], "Images/Menu/Btn_More_Game.png");
    Load_Texture(&Img_Menu_Lvl, "Images/Menu/Lvl.png");
    Load_Texture(&Img_Menu_Btn_Lvl_Act, "Images/Menu/Btn_Lvl_Act.png");
    Load_Texture(&Img_Menu_Btn_Lvl_Pas, "Images/Menu/Btn_Lvl_Pas.png");
    Load_Texture(&Img_Game_BG, "Images/Game/BG.png");
    Load_Texture(&Img_Switch, "Images/Game/Switch.png");
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
    Rct_Game_Back.Left = PADDING_START_X;
    Rct_Game_Back.Right = PADDING_FULL_X;
    Rct_Game_Back.Bottom = PADDING_START_Y;
    Rct_Game_Back.Top = PADDING_FULL_Y;
    Rct_Switch.Bottom = Rct_Switch.Left = -SWITCH_SIZE;
    Rct_Switch.Top = Rct_Switch.Right = SWITCH_SIZE;
    for (int i = 0; i < MAX_X; i++)
        Map[MAX_Y - 1][i] = Map[0][i] = CL_TILE_WALL;
    for (int i = 1; i < MAX_Y - 1; i++) {
        Map[i][0] = Map[i][MAX_X - 1] = CL_TILE_WALL;
        Map[i][1] = Map[i][MAX_X - 2] = CL_TILE_TRAP;
    }
    for (int i = 2; i < MAX_X - 2; i++)
        Map[MAX_Y - 2][i] = Map[1][i] = CL_TILE_TRAP;
    Load_Num();
    Load_Egg();
    Load_Enemy();
    Load_Tile();
    Load_Player();
    Init_Level();
    Init_Sound();
    Menu_Offset = MENU_BG_MAX_OFFSET;
    Game_Init = 0;
    Go_Menu();
}

void Menu_Display() {
    glClear(GL_COLOR_BUFFER_BIT);
    Map_Texture(&Img_Menu_BG);
    if (Menu_Offset >= WIDTH) {
        float l = 1 - Menu_Offset / MENU_BG_MAX_OFFSET;
        float r = l + 0.5f;
        glBegin(GL_POLYGON);
        glTexCoord2f(l, 0.0f);
        glVertex2f(0.0f, 0.0f);
        glTexCoord2f(r, 0.0f);
        glVertex2f(640.0f, 0.0f);
        glTexCoord2f(r, 1.0f);
        glVertex2f(640.0f, 480.0f);
        glTexCoord2f(l, 1.0f);
        glVertex2f(0.0f, 480.0f);
        glEnd();
    } else {
        float l = 1 - Menu_Offset / MENU_BG_MAX_OFFSET;
        float r = l - 0.5f;
        glBegin(GL_POLYGON);
        glTexCoord2f(l, 0.0f);
        glVertex2f(0.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(Menu_Offset, 0.0f);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(Menu_Offset, 480.0f);
        glTexCoord2f(l, 1.0f);
        glVertex2f(0.0f, 480.0f);
        glEnd();
        glBegin(GL_POLYGON);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(Menu_Offset, 0.0f);
        glTexCoord2f(r, 0.0f);
        glVertex2f(640.0f, 0.0f);
        glTexCoord2f(r, 1.0f);
        glVertex2f(640.0f, 480.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(Menu_Offset, 480.0f);
        glEnd();
    }
    Menu_Display_Func[Menu_Form_Stt]();
    glutSwapBuffers();
}

void Menu_Keyboard(unsigned char key, int x, int y) {
    if (key == 13) {
        switch (Menu_Form_Stt) {
        case MENU_STT_MAIN:
            if (Menu_Choice == 0) {
                Menu_Form_Stt = MENU_STT_LVL;
                Menu_Choice = 1;
                Set_Rect_Btn_Lvl();
            } else {
                // system("xdg-open https://www.youtube.com/channel/UCyRY53rs_lgyJ-p3PzZj0Og?sub_confirmation=1"); ubuntu
                system("start https://www.youtube.com/channel/UCyRY53rs_lgyJ-p3PzZj0Og?sub_confirmation=1");
            }
            break;
        case MENU_STT_LVL:
            Reload_Matrix_Game();
            Game_Level = Menu_Choice;
            Menu_Active = 0;
            Reload();
            glutDisplayFunc(Game_Display);
            break;
        }
    }
}

void Menu_Special(int key, int x, int y) {
    switch (Menu_Form_Stt) {
    case MENU_STT_MAIN:
        switch (key) {
        case GLUT_KEY_LEFT:
        case GLUT_KEY_RIGHT:
        case GLUT_KEY_DOWN:
        case GLUT_KEY_UP:
            Menu_Choice = 1 - Menu_Choice;
            break;
        }
        break;
    case MENU_STT_LVL:
        switch (key) {
        case GLUT_KEY_LEFT:
            Menu_Choice--;
            if (Menu_Choice % BTN_LVL_MAX_PER_ROW == 0) {
                Menu_Choice += BTN_LVL_MAX_PER_ROW;
                if (Menu_Choice > Menu_Max_Lvl)
                    Menu_Choice = Menu_Max_Lvl;
            }
            Set_Rect_Btn_Lvl();
            break;
        case GLUT_KEY_RIGHT:
            if (Menu_Choice % BTN_LVL_MAX_PER_ROW == 0)
                Menu_Choice -= BTN_LVL_MAX_PER_ROW - 1;
            else {
                Menu_Choice++;
                if (Menu_Choice > Menu_Max_Lvl)
                    Menu_Choice = Menu_Choice / BTN_LVL_MAX_PER_ROW * BTN_LVL_MAX_PER_ROW + 1;
            }
            Set_Rect_Btn_Lvl();
            break;
        case GLUT_KEY_DOWN:
            Menu_Choice += BTN_LVL_MAX_PER_ROW;
            if (Menu_Choice > Menu_Max_Lvl)
                Menu_Choice = (Menu_Choice - 1) % BTN_LVL_MAX_PER_ROW + 1;
            Set_Rect_Btn_Lvl();
            break;
        case GLUT_KEY_UP:
            Menu_Choice -= BTN_LVL_MAX_PER_ROW;
            if (Menu_Choice <= 0)
                Menu_Choice += BTN_LVL_MAX_REAL - (Menu_Choice + BTN_LVL_MAX_REAL - Menu_Max_Lvl + BTN_LVL_MAX_PER_ROW - 1) / BTN_LVL_MAX_PER_ROW * BTN_LVL_MAX_PER_ROW;
            Set_Rect_Btn_Lvl();
            break;
        }
        break;
    }
}

void Go_Menu() {
    Reload_Matrix_Menu();
    Menu_Active = 1;
    Menu_Stt = 0;
    Menu_Time = 0;
    Menu_Form_Stt = 0;
    Menu_Choice = 0;
    Menu_Alpha = 1.0f;
    Menu_Alpha_Offset = -0.03f;
    Game_Import = 1;
    Menu_Go_Alpha = 1.0f;
    Menu_Form_Stt = MENU_STT_GO;
    glutDisplayFunc(Menu_Display);
    glutKeyboardFunc(Menu_Keyboard);
    glutSpecialFunc(Menu_Special);
}
