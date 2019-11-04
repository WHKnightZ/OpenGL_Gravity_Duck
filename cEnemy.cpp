c_Enemy::c_Enemy(int x, int y) {
	this->x=x * TILE_SIZE + TILE_SIZE_HALF;
	this->y=y * TILE_SIZE + TILE_SIZE_HALF;
}

c_Enemy::~c_Enemy() {
}

void c_Enemy::Collision(){
	if (Game_Stt==GAME_STT_PLAY&&Player.x>Hitbox.Left&&Player.x<Hitbox.Right&&Player.y>Hitbox.Bottom&&Player.y<Hitbox.Top){
		Player.Set_Rct_Dead();
		Game_Stt=GAME_STT_DEAD;
		Player.Alpha = 1.0f;
	}
}

c_Enemy_Block_Hon::c_Enemy_Block_Hon(int x, int y, int Drt, int Move, int Move_Max):c_Enemy(x, y){
	if (Drt==0) vx=-2.0f;
	else vx=2.0f;
	this->Move=16*Move;
	this->Move_Max=16*Move_Max;
	Rct.Left=this->x-Enemy_Block_W/2;
	Rct.Right=Rct.Left+Enemy_Block_W;
	Rct.Bottom=this->y-Enemy_Block_H/2;
	Rct.Top=Rct.Bottom+Enemy_Block_H;
	Hitbox.Left=this->x-Enemy_Block_Hitbox_W;
	Hitbox.Right=this->x+Enemy_Block_Hitbox_W;
	Hitbox.Bottom=this->y-Enemy_Block_Hitbox_H;
	Hitbox.Top=this->y+Enemy_Block_Hitbox_H;
}

void c_Enemy_Block_Hon::Draw(){
	Map_Texture(&Img_Enemy_Block);
	Draw_Rect(&Rct);
}

void c_Enemy_Block_Hon::Action(){
	if (Move==Move_Max){
		Move=0;
		vx=-vx;
	}else{
		Move++;
		x+=vx;
		Rct.Left=x-Enemy_Block_W/2;
		Rct.Right=Rct.Left+Enemy_Block_W;
		Hitbox.Left=x-Enemy_Block_Hitbox_W;
		Hitbox.Right=x+Enemy_Block_Hitbox_W;
	}
	Collision();
}

c_Enemy_Block_Ver::c_Enemy_Block_Ver(int x, int y, int Drt, int Move, int Move_Max):c_Enemy(x, y){
	if (Drt==0) vy=-2.0f;
	else vy=2.0f;
	this->Move=16*Move;
	this->Move_Max=16*Move_Max;
	Rct.Left=this->x-Enemy_Block_W/2;
	Rct.Right=Rct.Left+Enemy_Block_W;
	Rct.Bottom=this->y-Enemy_Block_H/2;
	Rct.Top=Rct.Bottom+Enemy_Block_H;
	Hitbox.Left=this->x-Enemy_Block_Hitbox_W;
	Hitbox.Right=this->x+Enemy_Block_Hitbox_W;
	Hitbox.Bottom=this->y-Enemy_Block_Hitbox_H;
	Hitbox.Top=this->y+Enemy_Block_Hitbox_H;
}

void c_Enemy_Block_Ver::Draw(){
	Map_Texture(&Img_Enemy_Block);
	Draw_Rect(&Rct);
}

void c_Enemy_Block_Ver::Action(){
	if (Move==Move_Max){
		Move=0;
		vy=-vy;
	}else{
		Move++;
		y+=vy;
		Rct.Bottom=y-Enemy_Block_H/2;
		Rct.Top=Rct.Bottom+Enemy_Block_H;
		Hitbox.Bottom=y-Enemy_Block_Hitbox_H;
		Hitbox.Top=y+Enemy_Block_Hitbox_H;
	}
	Collision();
}
