c_Player::c_Player() {
}

c_Player::~c_Player() {
}

void c_Player::Import(int x, int y, int Gra) {
    s_x = x;
    s_y = y;
    s_Gra = Gra;
}

void c_Player::Reload() {
    x = s_x;
    y = s_y;
    Is_Run = Is_Jump = 0;
    vx = vy = 0.0f;
    Gra = s_Gra;
    Drt = 1;
    Stt = 0;
    gx = Gra_Offset[Gra].x;
    gy = Gra_Offset[Gra].y;
    Rct.Left = x - PLAYER_SIZE;
    Rct.Right = x + PLAYER_SIZE;
    Rct.Bottom = y - PLAYER_SIZE;
    Rct.Top = y + PLAYER_SIZE;
}

void c_Player::Set_Rct_Dead() {
    Rct_Dead.Left = Rct.Left - 4;
    Rct_Dead.Right = Rct_Dead.Left + 40;
    Rct_Dead.Bottom = Rct.Bottom - 4;
    Rct_Dead.Top = Rct_Dead.Bottom + 40;
}

void Load_Player() {
    int Pos[][4] = {
        {0, 0, 32, 32},
        {32, 0, 32, 32},
        {64, 0, 32, 32},
        {96, 0, 32, 32},
        {128, 0, 32, 32},
        {160, 0, 32, 32},
        {192, 0, 32, 32},
        {0, 32, 32, 32},
        {32, 32, 32, 32},
        {64, 32, 32, 32},
        {96, 32, 32, 32},
        {128, 32, 32, 32},
        {160, 32, 32, 32},
        {192, 32, 32, 32}};
    int *Ptr_Pos;
    Image Img;
    Image Img_Tmp;
    loadPng(&Img.img, &Img.w, &Img.h, "Images/Game/Player.png");
    for (int i = 0; i < 6; i++) {
        Ptr_Pos = &Pos[i][0];
        Crop_Image(&Img, &Img_Tmp, *Ptr_Pos, *(Ptr_Pos + 1), *(Ptr_Pos + 2), *(Ptr_Pos + 3));
        swapImage(Img_Tmp.img, Img_Tmp.w, Img_Tmp.h);
        Img_Player_Run[2][1][i] = Img_Tmp;
        Rotate_Left(&Img_Player_Run[2][1][i], &Img_Player_Run[3][0][i]);
        Rotate_180(&Img_Player_Run[2][1][i], &Img_Player_Run[0][0][i]);
        Rotate_180(&Img_Player_Run[3][0][i], &Img_Player_Run[1][1][i]);
        Flip_Horizontal(&Img_Player_Run[0][0][i], &Img_Player_Run[0][1][i]);
        Flip_Vertical(&Img_Player_Run[1][1][i], &Img_Player_Run[1][0][i]);
        Flip_Horizontal(&Img_Player_Run[2][1][i], &Img_Player_Run[2][0][i]);
        Flip_Vertical(&Img_Player_Run[3][0][i], &Img_Player_Run[3][1][i]);
    }
    for (int i = 0; i < 8; i++) {
        Ptr_Pos = &Pos[i + 6][0];
        Crop_Image(&Img, &Img_Tmp, *Ptr_Pos, *(Ptr_Pos + 1), *(Ptr_Pos + 2), *(Ptr_Pos + 3));
        swapImage(Img_Tmp.img, Img_Tmp.w, Img_Tmp.h);
        Img_Player_Spawn[2][1][i] = Img_Tmp;
        Rotate_Left(&Img_Player_Spawn[2][1][i], &Img_Player_Spawn[3][0][i]);
        Rotate_180(&Img_Player_Spawn[2][1][i], &Img_Player_Spawn[0][0][i]);
        Rotate_180(&Img_Player_Spawn[3][0][i], &Img_Player_Spawn[1][1][i]);
        Flip_Horizontal(&Img_Player_Spawn[0][0][i], &Img_Player_Spawn[0][1][i]);
        Flip_Vertical(&Img_Player_Spawn[1][1][i], &Img_Player_Spawn[1][0][i]);
        Flip_Horizontal(&Img_Player_Spawn[2][1][i], &Img_Player_Spawn[2][0][i]);
        Flip_Vertical(&Img_Player_Spawn[3][0][i], &Img_Player_Spawn[3][1][i]);
    }
    free(Img.img);
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 2; j++)
            Img_Player_Stand[i][j] = Img_Player_Spawn[i][j][7];
    loadPng(&Img.img, &Img.w, &Img.h, "Images/Game/Death.png");
    Img_Player_Death = Img;
}
