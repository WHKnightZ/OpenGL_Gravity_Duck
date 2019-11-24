void Load_Tile() {
    Image Img, Img_Tmp;
    loadPng(&Img.img, &Img.w, &Img.h, "Images/Game/Tiles.png");
    int x, y;
    for (int i = 0; i < TILE_MAX; i++) {
        x = (i % 11) * TILE_SIZE;
        y = (i / 11) * TILE_SIZE;
        Crop_Image(&Img, &Img_Tmp, x, y, TILE_SIZE, TILE_SIZE);
        swapImage(Img_Tmp.img, Img_Tmp.w, Img_Tmp.h);
        Imgd_Tile[i] = Img_Tmp.img;
    }
    free(Img.img);
}

void Create_Image_Game_Back() {
    if (Game_Init)
        free(Img_Game_Back.img);
    Clone_Image(&Img_Game_BG, &Img_Game_Back);
    Image Img;
    Img.w = Img.h = 32;
    for (int i = 2; i < MAX_Y_NO_PADDING; i++)
        for (int j = 2; j < MAX_X_NO_PADDING; j++)
            if (Map_Tile[i][j] > -1) {
                Img.img = Imgd_Tile[Map_Tile[i][j]];
                Mix_Image(&Img_Game_Back, &Img, (j - 2) * TILE_SIZE, (i - 2) * TILE_SIZE);
            }
    Game_Init = 1;
}

void Load_Egg() {
    int Pos[][6] = {
        {0, 0, 32, 32},
        {0, 32, 32, 32},
        {32, 0, 64, 64},
        {96, 0, 64, 64},
        {160, 0, 64, 64},
        {224, 0, 64, 64}};
    int *Ptr_Pos;
    Image Img;
    Image Img_Tmp;
    loadPng(&Img.img, &Img.w, &Img.h, "Images/Game/Egg.png");
    for (int i = 0; i < 2; i++) {
        Ptr_Pos = &Pos[i][0];
        Crop_Image(&Img, &Img_Tmp, *Ptr_Pos, *(Ptr_Pos + 1), *(Ptr_Pos + 2), *(Ptr_Pos + 3));
        swapImage(Img_Tmp.img, Img_Tmp.w, Img_Tmp.h);
        Img_Egg[2][i] = Img_Tmp;
    }
    for (int i = 2; i < 6; i++) {
        Ptr_Pos = &Pos[i][0];
        Crop_Image(&Img, &Img_Tmp, *Ptr_Pos, *(Ptr_Pos + 1), *(Ptr_Pos + 2), *(Ptr_Pos + 3));
        swapImage(Img_Tmp.img, Img_Tmp.w, Img_Tmp.h);
        Img_Egg_Pick[i - 2] = Img_Tmp;
    }
    free(Img.img);
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
    Player.Import(s_x + PADDING_START_X, s_y + PADDING_START_Y, s_Gra);
    fscanf(f, "%d%d%d", &dest_x, &dest_y, &Game_Dest_Gra);
    dest_x += 2;
    dest_y += 2;
    Max_X = MAX_X;
    Max_Y = MAX_Y;
    for (int i = 2; i < MAX_Y_NO_PADDING; i++)
        for (int j = 2; j < MAX_X_NO_PADDING; j++)
            fscanf(f, "%d", &Map_Tile[i][j]);
    int tmp, tmp2;
    int m = Max_Y / 2, n = Max_Y - 1;
    for (int i = 2; i < m; i++) {
        for (int j = 2; j < MAX_X_NO_PADDING; j++) {
            tmp = Map_Tile[i][j];
            Map_Tile[i][j] = Map_Tile[n - i][j];
            Map_Tile[n - i][j] = tmp;
        }
    }
    for (int i = 2; i < MAX_Y_NO_PADDING; i++) {
        for (int j = 2; j < MAX_X_NO_PADDING; j++) {
            if (Map_Tile[i][j] != -1)
                Map[i][j] = Tile_Mapping[Map_Tile[i][j]];
            else
                Map[i][j] = 1;
        }
    }
    Map[dest_y][dest_x] = CL_TILE_DEST;
    int i1, i2, i3, i4, i5, i6;
    fscanf(f, "%d", &Switch_Count);
    for (int i = 0; i < Switch_Count; i++) {
        fscanf(f, "%d%d%d", &i1, &i2, &i3);
        Switch[i].Set(i1 + 2, i2 + 2, i3);
    }
    fscanf(f, "%d", &Enemy_Count);
    for (int i = 0; i < Enemy_Count; i++) {
        fscanf(f, "%d", &tmp);
        switch (tmp) {
        case 0:
            fscanf(f, "%d%d%d%d%d", &i1, &i2, &i3, &i4, &i5);
            Enemy[i] = new c_Enemy_Block_Hon(i1 + 2, i2 + 2, i3, i4, i5);
            break;
        case 1:
            fscanf(f, "%d%d%d%d%d", &i1, &i2, &i3, &i4, &i5);
            Enemy[i] = new c_Enemy_Block_Ver(i1 + 2, i2 + 2, i3, i4, i5);
            break;
        case 2:
            fscanf(f, "%d%d%d%d%d%d", &i1, &i2, &i3, &i4, &i5, &i6);
            Enemy[i] = new c_Enemy_Worm(i1 + 2, i2 + 2, i3, i4, i5, i6);
            break;
        case 3:
            fscanf(f, "%d%d%d%d%d", &i1, &i2, &i3, &i4, &i5);
            Enemy[i] = new c_Enemy_Shooter(i1 + 2, i2 + 2, i3, i4, i5);
            break;
        case 4:
            fscanf(f, "%d%d%d%d", &i1, &i2, &i3, &i4);
            Enemy[i] = new c_Enemy_Fly(i1 + 2, i2 + 2, i3, i4);
            break;
        case 5:
            fscanf(f, "%d%d%d%d", &i1, &i2, &i3, &i4);
            Enemy[i] = new c_Enemy_Fly_Chase(i1 + 2, i2 + 2, i3, i4);
            break;
        case 6:
            fscanf(f, "%d%d%d%d%d", &i1, &i2, &i3, &i4, &i5);
            Enemy[i] = new c_Enemy_Crusher(i1 + 2, i2 + 2, i3, i4, i5);
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
    Create_Image_Game_Back();
    return 1;
}

void Reload() {
    if (Game_Import) {
        Free_Enemy_Bullet();
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
    Switch_Reload();
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
    Map_Texture(&Img_Game_Back);
    Draw_Rect(&Rct_Game_Back);
    Map_Texture(&Img_Switch);
    Switch_Draw();
    glLoadIdentity();
    Map_Texture(&Img_Egg[Game_Dest_Gra][Game_Stt_Egg]);
    Draw_Rect(&Rct_Egg);
    for (int i = 0; i < Enemy_Count; i++)
        Enemy[i]->Draw();
    s_List_Bullet *list = List_Bullet;
    while (list != NULL) {
        list->Bullet->Draw();
        list = list->next;
    }
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

void Timer(int value) {
    if (Menu_Active) {
        Menu_Process_Func[Menu_Form_Stt]();
    } else {
        Game_Process_Func[Game_Stt]();
        Switch_Action();
        Enemy_Action();
    }
    glutPostRedisplay();
    glutTimerFunc(INTERVAL, Timer, 0);
}
