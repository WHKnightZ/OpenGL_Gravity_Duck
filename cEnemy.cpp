c_Enemy::c_Enemy(int x, int y) {
    this->Stt = 0;
    this->x = x * TILE_SIZE + TILE_SIZE_HALF;
    this->y = y * TILE_SIZE + TILE_SIZE_HALF;
}

void c_Enemy::Collision() {
    if (Game_Stt == GAME_STT_PLAY && Player.x > Hitbox.Left && Player.x < Hitbox.Right && Player.y > Hitbox.Bottom && Player.y < Hitbox.Top) {
        Player.Set_Rct_Dead();
        Game_Stt = GAME_STT_DEAD;
        Mix_PlayChannel(-1, Sound_Dead, 0);
        Player.Alpha = 1.0f;
    }
}

c_Enemy_Block_Hon::c_Enemy_Block_Hon(int x, int y, int Drt, int Move, int Move_Max) : c_Enemy(x, y) {
    if (Drt == 0)
        vx = -2.0f;
    else
        vx = 2.0f;
    this->Move = 16 * Move;
    this->Move_Max = 16 * Move_Max;
    Rct.Left = this->x - Enemy_Block_W / 2;
    Rct.Right = Rct.Left + Enemy_Block_W;
    Rct.Bottom = this->y - Enemy_Block_H / 2;
    Rct.Top = Rct.Bottom + Enemy_Block_H;
    Hitbox.Left = this->x - Enemy_Block_Hitbox_W;
    Hitbox.Right = this->x + Enemy_Block_Hitbox_W;
    Hitbox.Bottom = this->y - Enemy_Block_Hitbox_H;
    Hitbox.Top = this->y + Enemy_Block_Hitbox_H;
}

void c_Enemy_Block_Hon::Draw() {
    Map_Texture(&Img_Enemy_Block);
    Draw_Rect(&Rct);
}

void c_Enemy_Block_Hon::Action() {
    if (Move == Move_Max) {
        Move = 0;
        vx = -vx;
    } else {
        Move++;
        x += vx;
        Rct.Left = x - Enemy_Block_W / 2;
        Rct.Right = Rct.Left + Enemy_Block_W;
        Hitbox.Left = x - Enemy_Block_Hitbox_W;
        Hitbox.Right = x + Enemy_Block_Hitbox_W;
    }
    Collision();
}

c_Enemy_Block_Ver::c_Enemy_Block_Ver(int x, int y, int Drt, int Move, int Move_Max) : c_Enemy(x, y) {
    if (Drt == 0)
        vy = -2.0f;
    else
        vy = 2.0f;
    this->Move = 16 * Move;
    this->Move_Max = 16 * Move_Max;
    Rct.Left = this->x - Enemy_Block_W / 2;
    Rct.Right = Rct.Left + Enemy_Block_W;
    Rct.Bottom = this->y - Enemy_Block_H / 2;
    Rct.Top = Rct.Bottom + Enemy_Block_H;
    Hitbox.Left = this->x - Enemy_Block_Hitbox_W;
    Hitbox.Right = this->x + Enemy_Block_Hitbox_W;
    Hitbox.Bottom = this->y - Enemy_Block_Hitbox_H;
    Hitbox.Top = this->y + Enemy_Block_Hitbox_H;
}

void c_Enemy_Block_Ver::Draw() {
    Map_Texture(&Img_Enemy_Block);
    Draw_Rect(&Rct);
}

void c_Enemy_Block_Ver::Action() {
    if (Move == Move_Max) {
        Move = 0;
        vy = -vy;
    } else {
        Move++;
        y += vy;
        Rct.Bottom = y - Enemy_Block_H / 2;
        Rct.Top = Rct.Bottom + Enemy_Block_H;
        Hitbox.Bottom = y - Enemy_Block_Hitbox_H;
        Hitbox.Top = y + Enemy_Block_Hitbox_H;
    }
    Collision();
}

c_Enemy_Worm::c_Enemy_Worm(int x, int y, int Gra, int Drt, int Move, int Move_Max) : c_Enemy(x, y) {
    this->Gra = Gra;
    this->Drt = Drt;
    Gra_Map = Gra_Mapping[Gra];
    if (Gra_Map == 0) {
        if (Drt == 0)
            vx = -2.0f;
        else
            vx = 2.0f;
    } else {
        if (Drt == 0)
            vy = -2.0f;
        else
            vy = 2.0f;
    }
    this->Move = 16 * Move;
    this->Move_Max = 16 * Move_Max;
    Rct.Left = this->x - Enemy_Worm_W / 2;
    Rct.Right = Rct.Left + Enemy_Worm_W;
    Rct.Bottom = this->y - Enemy_Worm_H / 2;
    Rct.Top = Rct.Bottom + Enemy_Worm_H;
    Hitbox.Left = this->x - Enemy_Worm_Hitbox_W;
    Hitbox.Right = this->x + Enemy_Worm_Hitbox_W;
    Hitbox.Bottom = this->y - Enemy_Worm_Hitbox_H;
    Hitbox.Top = this->y + Enemy_Worm_Hitbox_H;
}

void c_Enemy_Worm::Draw() {
    Map_Texture(&Img_Enemy_Worm[Gra][Drt][Stt]);
    Draw_Rect(&Rct);
}

void c_Enemy_Worm::Action() {
    if (Enemy_Time == 0)
        Stt = Loop_6[Stt];
    if (Gra_Map == 0) {
        if (Move == Move_Max) {
            Move = 0;
            vx = -vx;
            Drt = 1 - Drt;
        } else {
            Move++;
            x += vx;
            Rct.Left = x - Enemy_Worm_W / 2;
            Rct.Right = Rct.Left + Enemy_Worm_W;
            Hitbox.Left = x - Enemy_Worm_Hitbox_W;
            Hitbox.Right = x + Enemy_Worm_Hitbox_W;
        }
    } else {
        if (Move == Move_Max) {
            Move = 0;
            vy = -vy;
            Drt = 1 - Drt;
        } else {
            Move++;
            y += vy;
            Rct.Bottom = y - Enemy_Worm_H / 2;
            Rct.Top = Rct.Bottom + Enemy_Worm_H;
            Hitbox.Bottom = y - Enemy_Worm_Hitbox_H;
            Hitbox.Top = y + Enemy_Worm_Hitbox_H;
        }
    }
    Collision();
}

c_Enemy_Shooter::c_Enemy_Shooter(int x, int y, int Drt, int Move, int Move_Max) : c_Enemy(x, y) {
    this->Drt = Drt;
    this->Move = Move;
    this->Move_Max = Move_Max;
    Rct.Left = this->x - Enemy_Shooter_W / 2;
    Rct.Right = Rct.Left + Enemy_Shooter_W;
    Rct.Bottom = this->y - Enemy_Shooter_H / 2;
    Rct.Top = Rct.Bottom + Enemy_Shooter_H;
    Hitbox.Left = this->x - Enemy_Shooter_Hitbox_W;
    Hitbox.Right = this->x + Enemy_Shooter_Hitbox_W;
    Hitbox.Bottom = this->y - Enemy_Shooter_Hitbox_H;
    Hitbox.Top = this->y + Enemy_Shooter_Hitbox_H;
}

void c_Enemy_Shooter::Draw() {
    Map_Texture(&Img_Enemy_Shooter[Drt][Stt]);
    Draw_Rect(&Rct);
}

void c_Enemy_Shooter::Action() {
    if (Move == Move_Max) {
        if (Enemy_Time == 0) {
            Stt++;
            if (Stt == 3) {
                Stt = 0;
                Move = 0;
                List_Bullet_Add(new c_Bullet_Shooter(x + Bullet_Shooter_Offset[Drt], y + 1.0f, Bullet_Shooter_Velocity[Drt]));
            }
        }
    } else {
        Move++;
    }
    Collision();
}

c_Enemy_Fly::c_Enemy_Fly(int x, int y, int Move, int Move_Max) : c_Enemy(x, y) {
    Stt_Fire = 0;
    this->Move = Move;
    this->Move_Max = Move_Max;
    if (Player.x < this->x)
        Drt = 0;
    else
        Drt = 1;
    Rct.Left = this->x - Enemy_Fly_W / 2;
    Rct.Right = Rct.Left + Enemy_Fly_W;
    Rct.Bottom = this->y - Enemy_Fly_H / 2;
    Rct.Top = Rct.Bottom + Enemy_Fly_H;
    Rct_Fire.Left = this->x - Enemy_Fly_Fire_W / 2;
    Rct_Fire.Right = Rct_Fire.Left + Enemy_Fly_Fire_W;
    Rct_Fire.Bottom = this->y - Enemy_Fly_Fire_H / 2;
    Rct_Fire.Top = Rct_Fire.Bottom + Enemy_Fly_Fire_H;
    Hitbox.Left = this->x - Enemy_Fly_Hitbox_W;
    Hitbox.Right = this->x + Enemy_Fly_Hitbox_W;
    Hitbox.Bottom = this->y - Enemy_Fly_Hitbox_H;
    Hitbox.Top = this->y + Enemy_Fly_Hitbox_H;
}

void c_Enemy_Fly::Draw() {
    Map_Texture(&Img_Enemy_Fly[Drt][Stt]);
    Draw_Rect(&Rct);
    if (Move == Move_Max) {
        Map_Texture(&Img_Enemy_Fly_Fire[Stt_Fire]);
        Draw_Rect(&Rct_Fire);
    }
}

void c_Enemy_Fly::Action() {
    if (Player.x < this->x)
        Drt = 0;
    else
        Drt = 1;
    if (Enemy_Time == 0)
        Stt = Loop_4[Stt];
    if (Move == Move_Max) {
        Stt_Fire++;
        if (Stt_Fire == 16) {
            Tmp_Dis_X = Player.x - x;
            Tmp_Dis_Y = Player.y - y;
            Tmp_Dis = sqrt(Tmp_Dis_X * Tmp_Dis_X + Tmp_Dis_Y * Tmp_Dis_Y);
            Tmp_Vx = Tmp_Dis_X / Tmp_Dis;
            Tmp_Vy = Tmp_Dis_Y / Tmp_Dis;
            List_Bullet_Add(new c_Bullet_Fly(x + Bullet_Fly_Offset * Tmp_Vx, y + Bullet_Fly_Offset * Tmp_Vy, Bullet_Fly_Velocity * Tmp_Vx, Bullet_Fly_Velocity * Tmp_Vy, atan2(-Tmp_Dis_X, Tmp_Dis_Y) * 57.2958f));
        } else if (Stt_Fire == 21) {
            Move = 0;
            Stt_Fire = 0;
        }
    } else {
        Move++;
    }
    Collision();
}

void Load_Enemy_Worm() {
    int Pos[][4] = {
        {0, 0, 32, 32},
        {32, 0, 32, 32},
        {0, 32, 32, 32},
        {32, 32, 32, 32}};
    int *Ptr_Pos;
    Image Img;
    Image Img_Tmp;
    loadPng(&Img.img, &Img.w, &Img.h, "Images/Enemies/Worm.png");
    for (int i = 0; i < 4; i++) {
        Ptr_Pos = &Pos[i][0];
        Crop_Image(&Img, &Img_Tmp, *Ptr_Pos, *(Ptr_Pos + 1), *(Ptr_Pos + 2), *(Ptr_Pos + 3));
        swapImage(Img_Tmp.img, Img_Tmp.w, Img_Tmp.h);
        Img_Enemy_Worm[2][0][i] = Img_Tmp;
        Rotate_Left(&Img_Enemy_Worm[2][0][i], &Img_Enemy_Worm[3][1][i]);
        Rotate_180(&Img_Enemy_Worm[2][0][i], &Img_Enemy_Worm[0][1][i]);
        Rotate_180(&Img_Enemy_Worm[3][1][i], &Img_Enemy_Worm[1][0][i]);
        Flip_Horizontal(&Img_Enemy_Worm[0][1][i], &Img_Enemy_Worm[0][0][i]);
        Flip_Vertical(&Img_Enemy_Worm[1][0][i], &Img_Enemy_Worm[1][1][i]);
        Flip_Horizontal(&Img_Enemy_Worm[2][0][i], &Img_Enemy_Worm[2][1][i]);
        Flip_Vertical(&Img_Enemy_Worm[3][1][i], &Img_Enemy_Worm[3][0][i]);
    }
    free(Img.img);
    for (int i = 4; i < 6; i++) {
        for (int j = 0; j < 4; j++) {
            Img_Enemy_Worm[j][0][i] = Img_Enemy_Worm[j][0][6 - i];
            Img_Enemy_Worm[j][1][i] = Img_Enemy_Worm[j][1][6 - i];
        }
    }
}

void Load_Enemy_Shooter() {
    int Pos[][4] = {
        {0, 0, 32, 32},
        {32, 0, 32, 32},
        {64, 0, 32, 32},
        {96, 0, 8, 8}};
    int *Ptr_Pos;
    Image Img;
    Image Img_Tmp;
    loadPng(&Img.img, &Img.w, &Img.h, "Images/Enemies/Shooter.png");
    for (int i = 0; i < 3; i++) {
        Ptr_Pos = &Pos[i][0];
        Crop_Image(&Img, &Img_Tmp, *Ptr_Pos, *(Ptr_Pos + 1), *(Ptr_Pos + 2), *(Ptr_Pos + 3));
        swapImage(Img_Tmp.img, Img_Tmp.w, Img_Tmp.h);
        Img_Enemy_Shooter[0][i] = Img_Tmp;
        Flip_Horizontal(&Img_Enemy_Shooter[0][i], &Img_Enemy_Shooter[1][i]);
    }
    Ptr_Pos = &Pos[3][0];
    Crop_Image(&Img, &Img_Tmp, *Ptr_Pos, *(Ptr_Pos + 1), *(Ptr_Pos + 2), *(Ptr_Pos + 3));
    swapImage(Img_Tmp.img, Img_Tmp.w, Img_Tmp.h);
    Img_Bullet_Shooter = Img_Tmp;
    free(Img.img);
}

void Load_Enemy_Fly() {
    int Pos[][4] = {
        {0, 0, 32, 32},
        {32, 0, 32, 32},
        {0, 32, 32, 32},
        {32, 32, 32, 32},
        {64, 0, 64, 64},
        {128, 0, 64, 64},
        {192, 0, 64, 64},
        {256, 0, 64, 64},
        {0, 64, 64, 64},
        {64, 64, 64, 64},
        {128, 64, 64, 64},
        {192, 64, 64, 64},
        {256, 64, 64, 64},
        {320, 0, 4, 8},
        {320, 8, 24, 24}};
    int *Ptr_Pos;
    Image Img;
    Image Img_Tmp;
    loadPng(&Img.img, &Img.w, &Img.h, "Images/Enemies/Fly.png");
    for (int i = 0; i < 4; i++) {
        Ptr_Pos = &Pos[i][0];
        Crop_Image(&Img, &Img_Tmp, *Ptr_Pos, *(Ptr_Pos + 1), *(Ptr_Pos + 2), *(Ptr_Pos + 3));
        swapImage(Img_Tmp.img, Img_Tmp.w, Img_Tmp.h);
        Img_Enemy_Fly[0][i] = Img_Tmp;
        Flip_Horizontal(&Img_Enemy_Fly[0][i], &Img_Enemy_Fly[1][i]);
    }
    for (int i = 0; i < 4; i++) {
        Ptr_Pos = &Pos[i + 4][0];
        Crop_Image(&Img, &Img_Tmp, *Ptr_Pos, *(Ptr_Pos + 1), *(Ptr_Pos + 2), *(Ptr_Pos + 3));
        Img_Enemy_Fly_Fire[i] = Img_Tmp;
    }
    Img_Enemy_Fly_Fire[4] = Img_Enemy_Fly_Fire[2];
    Img_Enemy_Fly_Fire[5] = Img_Enemy_Fly_Fire[1];
    Img_Enemy_Fly_Fire[6] = Img_Enemy_Fly_Fire[0];
    Img_Enemy_Fly_Fire[7] = Img_Enemy_Fly_Fire[1];
    Img_Enemy_Fly_Fire[8] = Img_Enemy_Fly_Fire[2];
    Img_Enemy_Fly_Fire[9] = Img_Enemy_Fly_Fire[3];
    Img_Enemy_Fly_Fire[10] = Img_Enemy_Fly_Fire[2];
    Img_Enemy_Fly_Fire[11] = Img_Enemy_Fly_Fire[1];
    Img_Enemy_Fly_Fire[12] = Img_Enemy_Fly_Fire[0];
    Img_Enemy_Fly_Fire[13] = Img_Enemy_Fly_Fire[1];
    Img_Enemy_Fly_Fire[14] = Img_Enemy_Fly_Fire[2];
    Img_Enemy_Fly_Fire[15] = Img_Enemy_Fly_Fire[3];
    for (int i = 0; i < 5; i++) {
        Ptr_Pos = &Pos[i + 8][0];
        Crop_Image(&Img, &Img_Tmp, *Ptr_Pos, *(Ptr_Pos + 1), *(Ptr_Pos + 2), *(Ptr_Pos + 3));
        Img_Enemy_Fly_Fire[i + 16] = Img_Tmp;
    }
    Ptr_Pos = &Pos[13][0];
    Crop_Image(&Img, &Img_Tmp, *Ptr_Pos, *(Ptr_Pos + 1), *(Ptr_Pos + 2), *(Ptr_Pos + 3));
    swapImage(Img_Tmp.img, Img_Tmp.w, Img_Tmp.h);
    Img_Bullet_Fly = Img_Tmp;
    Rct_Bullet_Fly.Left = -Bullet_Fly_W / 2.0f;
    Rct_Bullet_Fly.Right = Rct_Bullet_Fly.Left + Bullet_Fly_W;
    Rct_Bullet_Fly.Bottom = -Bullet_Fly_H / 2.0f;
    Rct_Bullet_Fly.Top = Rct_Bullet_Fly.Bottom + Bullet_Fly_H;
    Ptr_Pos = &Pos[14][0];
    Crop_Image(&Img, &Img_Tmp, *Ptr_Pos, *(Ptr_Pos + 1), *(Ptr_Pos + 2), *(Ptr_Pos + 3));
    Img_Bullet_Fly_Explode = Img_Tmp;
    free(Img.img);
}

void Load_Enemy() {
    Load_Texture(&Img_Enemy_Block, "Images/Enemies/Block.png");
    Load_Enemy_Worm();
    Load_Enemy_Shooter();
    Load_Enemy_Fly();
}

void Enemy_Action() {
    Enemy_Time = Loop_Time[Enemy_Time];
    for (int i = 0; i < Enemy_Count; i++)
        Enemy[i]->Action();
    s_List_Bullet *list = List_Bullet, *tmp = NULL;
    while (list != NULL) {
        list->Bullet->Action();
        if (!list->Bullet->Is_Alive) {
            if (tmp != NULL) {
                tmp->next = list->next;
                delete list;
                list = tmp->next;
            } else {
                List_Bullet = list->next;
                delete list;
                list = List_Bullet;
            }
        } else {
            tmp = list;
            list = list->next;
        }
    }
}

void Free_Enemy_Bullet() {
    for (int i = 0; i < Enemy_Count; i++)
        delete Enemy[i];
    Enemy_Count = 0;
    s_List_Bullet *list = List_Bullet;
    while (List_Bullet != NULL) {
        List_Bullet = List_Bullet->next;
        delete list;
        list = List_Bullet;
    }
}
