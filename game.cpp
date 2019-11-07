void Load_Tile() {
    Image Img;
    for (int i = 0; i < TILE_MAX; i++) {
        sprintf(Str, "Images/Tiles/%02d.png", i);
        Load_Texture(&Img, Str);
        Imgd_Tile[i] = Img.img;
    }
}

void Create_List_Draw_Tile() {
    if (!Game_Init)
        glDeleteLists(1, 1);
    glNewList(1, GL_COMPILE);
    Map_Texture(&Img_Game_BG);
    Rct.Left = 0;
    Rct.Right = WIDTH;
    Rct.Bottom = 0;
    Rct.Top = HEIGHT;
    Draw_Rect(&Rct);
    for (int i = 0; i < Max_Y; i++) {
        for (int j = 0; j < Max_X; j++) {
            if (Map_Tile[i][j] > -1) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TILE_SIZE, TILE_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, Imgd_Tile[Map_Tile[i][j]]);
                Rct.Left = j * TILE_SIZE;
                Rct.Right = Rct.Left + TILE_SIZE;
                Rct.Bottom = i * TILE_SIZE;
                Rct.Top = Rct.Bottom + TILE_SIZE;
                Draw_Rect(&Rct);
            }
        }
    }
    glEndList();
}

void Load_Egg() {
    Load_Texture(&Img_Egg[2][0], "Images/Egg.png");
    Load_Texture(&Img_Egg[2][1], "Images/Egg_Null.png");
    Load_Texture(&Img_Egg_Pick[0], "Images/Egg_Pick_0.png");
    Load_Texture(&Img_Egg_Pick[1], "Images/Egg_Pick_1.png");
    Load_Texture(&Img_Egg_Pick[2], "Images/Egg_Pick_2.png");
    Load_Texture(&Img_Egg_Pick[3], "Images/Egg_Pick_3.png");
    Rotate_Left(&Img_Egg[2][0], &Img_Egg[3][0]);
    Rotate_180(&Img_Egg[2][0], &Img_Egg[0][0]);
    Rotate_180(&Img_Egg[3][0], &Img_Egg[1][0]);
    Rotate_Left(&Img_Egg[2][1], &Img_Egg[3][1]);
    Rotate_180(&Img_Egg[2][1], &Img_Egg[0][1]);
    Rotate_180(&Img_Egg[3][1], &Img_Egg[1][1]);
}

int Import_Map(int Level) {
    sprintf(Str, "Maps/%02d.txt", Level);
    FILE *f = fopen(Str, "r");
    if (f == NULL)
        return 0;
    int s_x, s_y, s_Gra, dest_x, dest_y;
    fscanf(f, "%d%d%d", &s_x, &s_y, &s_Gra);
    Player.Import(s_x, s_y, s_Gra);
    fscanf(f, "%d%d%d", &dest_x, &dest_y, &Game_Dest_Gra);
    Max_X = 20;
    Max_Y = 15;
    for (int i = 0; i < Max_Y; i++)
        for (int j = 0; j < Max_X; j++)
            fscanf(f, "%d", &Map_Tile[i][j]);
    int tmp, tmp2;
    int m = Max_Y / 2, n = Max_Y - 1;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < Max_X; j++) {
            tmp = Map_Tile[i][j];
            Map_Tile[i][j] = Map_Tile[n - i][j];
            Map_Tile[n - i][j] = tmp;
        }
    }
    for (int i = 0; i < Max_Y; i++) {
        for (int j = 0; j < Max_X; j++) {
            if (Map_Tile[i][j] != -1)
                Map[i][j] = Tile_Mapping[Map_Tile[i][j]];
            else
                Map[i][j] = 1;
        }
    }
    Map[dest_y][dest_x] = CL_TILE_DEST;
    fscanf(f, "%d", &Switch_Count);
    for (int i = 0; i < Switch_Count; i++) {
        fscanf(f, "%d%d", &tmp, &tmp2);
        Switch[i].Set(tmp * TILE_SIZE + TILE_SIZE_HALF, tmp2 * TILE_SIZE + TILE_SIZE_HALF);
    }
    fscanf(f, "%d", &Enemy_Count);
    int i1, i2, i3, i4, i5, i6;
    for (int i = 0; i < Enemy_Count; i++) {
        fscanf(f, "%d", &tmp);
        switch (tmp) {
        case 0:
            fscanf(f, "%d%d%d%d%d", &i1, &i2, &i3, &i4, &i5);
            Enemy[i] = new c_Enemy_Block_Hon(i1, i2, i3, i4, i5);
            break;
        case 1:
            fscanf(f, "%d%d%d%d%d", &i1, &i2, &i3, &i4, &i5);
            Enemy[i] = new c_Enemy_Block_Ver(i1, i2, i3, i4, i5);
            break;
        case 2:
            fscanf(f, "%d%d%d%d%d%d", &i1, &i2, &i3, &i4, &i5, &i6);
            Enemy[i] = new c_Enemy_Worm(i1, i2, i3, i4, i5, i6);
            break;
        }
    }
    int x = dest_x * TILE_SIZE + TILE_SIZE_HALF, y = dest_y * TILE_SIZE + TILE_SIZE_HALF;
    Rct_Egg.Left = x - Img_Egg[Game_Dest_Gra][0].w / 2;
    Rct_Egg.Bottom = y - Img_Egg[Game_Dest_Gra][0].h / 2;
    Rct_Egg.Right = Rct_Egg.Left + Img_Egg[Game_Dest_Gra][0].w;
    Rct_Egg.Top = Rct_Egg.Bottom + Img_Egg[Game_Dest_Gra][0].h;
    Rct_Egg_Pick.Left = x - Img_Egg_Pick[0].w / 2;
    Rct_Egg_Pick.Bottom = y - Img_Egg_Pick[0].h / 2;
    Rct_Egg_Pick.Right = Rct_Egg_Pick.Left + Img_Egg_Pick[0].w;
    Rct_Egg_Pick.Top = Rct_Egg_Pick.Bottom + Img_Egg_Pick[0].h;
    fclose(f);
    Create_List_Draw_Tile();
    return 1;
}

void Reload() {
    if (Game_Import) {
        Game_Import = 0;
        if (!Import_Map(Game_Level)) {
            Go_Menu();
            return;
        }
    }
    Game_Stt = GAME_STT_BEGIN;
    Game_Alpha = 1.0f;
    Game_Time = 0;
    Game_Stt_Egg = 0;
    Player.Reload();
}

void Draw_Rect(Rect *Ptr_Rct) {
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(Ptr_Rct->Left, Ptr_Rct->Bottom);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(Ptr_Rct->Right, Ptr_Rct->Bottom);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(Ptr_Rct->Right, Ptr_Rct->Top);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(Ptr_Rct->Left, Ptr_Rct->Top);
    glEnd();
}

void Game_Display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glCallList(1);
    Map_Texture(&Img_Switch);
    Switch_Draw();
    glLoadIdentity();
    Map_Texture(&Img_Egg[Game_Dest_Gra][Game_Stt_Egg]);
    Draw_Rect(&Rct_Egg);
    for (int i = 0; i < Enemy_Count; i++)
        Enemy[i]->Draw();
    Game_Display_Func[Game_Stt]();
    glutSwapBuffers();
}

void Game_Keyboard(unsigned char key, int x, int y) {
    if (Game_Stt == GAME_STT_PLAY && key == 32 && !Player.Is_Jump) {
        Player.Gra = Gra_Reverse[Player.Gra];
        Player.gx = Gra_Offset[Player.Gra].x;
        Player.gy = Gra_Offset[Player.Gra].y;
        Mix_PlayChannel(-1, Sound_Switch, 0);
    } else if (key == 27)
        Go_Menu();
}

void Game_Special(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_LEFT:
        if (Gra_Mapping[Player.Gra] == 0) {
            Player.Drt = 0;
            Player.vx = -5.0f;
        }
        break;
    case GLUT_KEY_RIGHT:
        if (Gra_Mapping[Player.Gra] == 0) {
            Player.Drt = 1;
            Player.vx = 5.0f;
        }
        break;
    case GLUT_KEY_DOWN:
        if (Gra_Mapping[Player.Gra] == 1) {
            Player.Drt = 0;
            Player.vy = -5.0f;
        }
        break;
    case GLUT_KEY_UP:
        if (Gra_Mapping[Player.Gra] == 1) {
            Player.Drt = 1;
            Player.vy = 5.0f;
        }
        break;
    }
}

void Game_Special_Up(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_LEFT:
        if (Player.Drt == 0 && Gra_Mapping[Player.Gra] == 0)
            Player.vx = 0.0f;
        break;
    case GLUT_KEY_RIGHT:
        if (Player.Drt == 1 && Gra_Mapping[Player.Gra] == 0)
            Player.vx = 0.0f;
        break;
    case GLUT_KEY_DOWN:
        if (Player.Drt == 0 && Gra_Mapping[Player.Gra] == 1)
            Player.vy = 0.0f;
        break;
    case GLUT_KEY_UP:
        if (Player.Drt == 1 && Gra_Mapping[Player.Gra] == 1)
            Player.vy = 0.0f;
        break;
    }
}

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
    Load_Texture(&Img_Menu_BG, "Images/Menu/BG.png");
    Load_Texture(&Img_Menu_Main, "Images/Menu/Main.png");
    Load_Texture(&Img_Menu_Btn[0], "Images/Menu/Btn_Start_Game.png");
    Load_Texture(&Img_Menu_Btn[1], "Images/Menu/Btn_More_Game.png");
    Load_Texture(&Img_Menu_Lvl, "Images/Menu/Lvl.png");
    Load_Texture(&Img_Menu_Btn_Lvl_Act, "Images/Menu/Btn_Lvl_Act.png");
    Load_Texture(&Img_Menu_Btn_Lvl_Pas, "Images/Menu/Btn_Lvl_Pas.png");
    Load_Texture(&Img_Game_BG, "Images/Game_BG.png");
    Load_Texture(&Img_Switch, "Images/Switch.png");
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

void Timer(int value) {
    if (Menu_Active) {
        Menu_Process_Func[Menu_Form_Stt]();
    } else {
        Game_Process_Func[Game_Stt]();
        Switch_Action();
        for (int i = 0; i < Enemy_Count; i++)
            Enemy[i]->Action();
    }
    glutPostRedisplay();
    glutTimerFunc(INTERVAL, Timer, 0);
}
