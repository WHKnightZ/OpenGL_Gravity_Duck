c_Enemy::c_Enemy(int x, int y) {
    this->x = x * TILE_SIZE + TILE_SIZE_HALF;
    this->y = y * TILE_SIZE + TILE_SIZE_HALF;
}

c_Enemy::~c_Enemy() {
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
	if (Gra_Mapping[Gra] == 0)
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
	this->Gra=Gra;
	Gra_Map=Gra_Mapping[Gra];
	if (Gra_Map==0){
		if (Drt == 0)
	        vx = -4.0f;
	    else
	        vx = 4.0f;
	} else {
		if (Drt == 0)
	        vy = -4.0f;
	    else
	        vy = 4.0f;
	}
    this->Move = 8 * Move;
    this->Move_Max = 8 * Move_Max;
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
    Map_Texture(&Img_Enemy_Worm[2][0][1]);
    Draw_Rect(&Rct);
}

void c_Enemy_Worm::Action(){
	if (Game_Time==0) Stt=Loop_6[Stt];
	if (Gra_Map==0){
		if (Move == Move_Max) {
	        Move = 0;
	        vx = -vx;
	        Drt=1-Drt;
	    } else {
	        Move++;
	        x += vx;
	        Rct.Left = x - Enemy_Worm_W / 2;
	        Rct.Right = Rct.Left + Enemy_Worm_W;
	        Hitbox.Left = x - Enemy_Worm_Hitbox_W;
	        Hitbox.Right = x + Enemy_Worm_Hitbox_W;
	    }
	} else{
		if (Move == Move_Max) {
	        Move = 0;
	        vy = -vy;
	        Drt=1-Drt;
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

void Load_Enemy(){
	Load_Texture(&Img_Enemy_Block, "Images/Enemys/Block.png");
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
        Img_Enemy_Worm[2][1][i] = Img_Tmp;
        printf("%d\n", Img_Enemy_Worm[2][1][0].h);
        Rotate_Left(&Img_Enemy_Worm[2][0][i], &Img_Enemy_Worm[3][1][i]);
        Rotate_180(&Img_Enemy_Worm[2][0][i], &Img_Enemy_Worm[0][1][i]);
        Rotate_180(&Img_Enemy_Worm[3][1][i], &Img_Enemy_Worm[1][0][i]);
        Flip_Horizontal(&Img_Enemy_Worm[0][1][i], &Img_Enemy_Worm[0][0][i]);
        Flip_Vertical(&Img_Enemy_Worm[1][0][i], &Img_Enemy_Worm[1][1][i]);
        Flip_Horizontal(&Img_Enemy_Worm[2][0][i], &Img_Enemy_Worm[2][1][i]);
        Flip_Vertical(&Img_Enemy_Worm[3][1][i], &Img_Enemy_Worm[3][0][i]);
    }
    for (int i=4;i<6;i++){
    	for (int j=0;j<4;j++){
    		Img_Enemy_Worm[j][0][i]=Img_Enemy_Worm[j][0][6-i];
    		Img_Enemy_Worm[j][1][i]=Img_Enemy_Worm[j][1][6-i];
		}
	}
}
