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
