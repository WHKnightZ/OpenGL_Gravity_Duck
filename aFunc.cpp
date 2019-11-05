void Menu_Display_Main() {
    Map_Texture(&Img_Menu_Main);
    Draw_Rect(&Rct_Menu_Main);
    glColor3f(Menu_Alpha, Menu_Alpha, Menu_Alpha);
    Map_Texture(&Img_Menu_Btn[Menu_Choice]);
    Draw_Rect(&Rct_Menu_Btn[Menu_Choice]);
    glColor3f(1.0f, 1.0f, 1.0f);
}

void Menu_Display_Lvl() {
    Map_Texture(&Img_Menu_Lvl);
    Draw_Rect(&Rct_Menu_Lvl);
    glColor3f(Menu_Alpha, Menu_Alpha, Menu_Alpha);
    Map_Texture(&Img_Menu_Btn_Lvl[Menu_Choice]);
    Draw_Rect(&Rct_Menu_Btn_Lvl);
    glColor3f(1.0f, 1.0f, 1.0f);
}

void Game_Display_Begin_End() {
    glDisable(GL_TEXTURE_2D);
    glColor4f(0.0f, 0.0f, 0.0f, Game_Alpha);
    glBegin(GL_POLYGON);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(WIDTH, 0.0f);
    glVertex2f(WIDTH, HEIGHT);
    glVertex2f(0.0f, HEIGHT);
    glEnd();
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
}

void Game_Display_Spawn_Win() {
    Map_Texture(&Img_Player_Spawn[Player.Gra][Player.Drt][Player.Stt]);
    Draw_Rect(&Player.Rct);
}

void Game_Display_Play() {
    if (Player.Is_Jump)
        Map_Texture(&Img_Player_Stand[Player.Gra][Player.Drt]);
    else if (Player.Is_Run)
        Map_Texture(&Img_Player_Run[Player.Gra][Player.Drt][Player.Stt]);
    else
        Map_Texture(&Img_Player_Stand[Player.Gra][Player.Drt]);
    Draw_Rect(&Player.Rct);
}

void Game_Display_Dead() {
    glColor4f(1.0f, 1.0f, 1.0f, Player.Alpha);
    Map_Texture(&Img_Player_Death);
    Draw_Rect(&Player.Rct_Dead);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void Game_Display_Pick(){
	Game_Display_Play();
	Map_Texture(&Img_Egg_Pick[Game_Stt_Egg_Pick]);
	Draw_Rect(&Rct_Egg_Pick);
}

void Game_Process_Begin() {
    Game_Alpha -= 0.05f;
    if (Game_Alpha <= 0.0f) {
        Game_Stt = GAME_STT_SPAWN;
    }
}

void Game_Process_Spawn() {
    Game_Time = Loop_Time[Game_Time];
    if (Game_Time == 0) {
        Player.Stt++;
        if (Player.Stt == 8) {
            Player.Stt = 0;
            Game_Stt = GAME_STT_PLAY;
            glutKeyboardFunc(Game_Keyboard);
            glutSpecialFunc(Game_Special);
            glutSpecialUpFunc(Game_Special_Up);
        }
    }
}

void Game_Process_Play() {
    Game_Time = Loop_Time[Game_Time];
    if (Game_Time == 0)
        Player.Stt = Loop_Stt[Player.Stt];
    Player.vx += Player.gx;
    Player.vy += Player.gy;
    i_vx = (int)Player.vx;
    i_vy = (int)Player.vy;
    Player.Is_Run = Player.Is_Jump = 0;
    if (i_vx < 0)
        Player_Move_Left();
    else if (i_vx > 0)
        Player_Move_Right();
    if (i_vy < 0)
        Player_Move_Down();
    else if (i_vy > 0)
        Player_Move_Up();
    Collision_Tile();
}

void Game_Process_Dead() {
    Player.Alpha -= 0.05f;
    if (Player.Alpha <= 0.0f) {
        Game_Import = 0;
        Game_Alpha = 0.0f;
        Game_Stt = GAME_STT_END;
    }
}

void Game_Process_Pick(){
    Player.vx += Player.gx;
    Player.vy += Player.gy;
    i_vx = (int)Player.vx;
    i_vy = (int)Player.vy;
    if (i_vx < 0)
        Player_Move_Left();
    else if (i_vx > 0)
        Player_Move_Right();
    if (i_vy < 0)
        Player_Move_Down();
    else if (i_vy > 0)
        Player_Move_Up();
    Game_Time = Loop_Time[Game_Time];
    if (Game_Time == 0){
    	Game_Stt_Egg_Pick++;
    	if (Game_Stt_Egg_Pick==4){
    		Game_Stt = GAME_STT_WIN;
        	Player.Stt = 7;
		}
	}
}

void Game_Process_Win() {
    Game_Time = Loop_Time[Game_Time];
    if (Game_Time == 0) {
        Player.Stt--;
        if (Player.Stt == 0) {
            Game_Import = 1;
            Game_Level++;
            Game_Alpha = 0.0f;
            Game_Stt = GAME_STT_END;
        }
    }
}

void Game_Process_End() {
    Game_Alpha += 0.05f;
    if (Game_Alpha >= 1.0f) {
        Reload();
        Game_Alpha = 1.0f;
        Game_Stt = GAME_STT_BEGIN;
    }
}

void Collision_Tile_Wall() {}

void Collision_Tile_Nothing() {}

void Collision_Tile_Trap() {
    Player.Set_Rct_Dead();
    Game_Stt = GAME_STT_DEAD;
    Player.Alpha = 1.0f;
}

void Collision_Tile_Dest() {
    if (Player.Gra == Game_Dest_Gra && Game_Stt != GAME_STT_PICK && Game_Stt != GAME_STT_WIN) {
    	Game_Stt_Egg=1;
    	Game_Stt_Egg_Pick=0;
        Game_Stt = GAME_STT_PICK;
        Player.vx = Player.vy =0.0f;
        glutKeyboardFunc(NULL);
        glutSpecialFunc(NULL);
        glutSpecialUpFunc(NULL);
    }
}

void Init_Array_Func() {
    Menu_Display_Func[MENU_STT_MAIN] = Menu_Display_Main;
    Menu_Display_Func[MENU_STT_LVL] = Menu_Display_Lvl;
    Game_Display_Func[GAME_STT_BEGIN] = Game_Display_Begin_End;
    Game_Display_Func[GAME_STT_SPAWN] = Game_Display_Spawn_Win;
    Game_Display_Func[GAME_STT_PLAY] = Game_Display_Play;
    Game_Display_Func[GAME_STT_DEAD] = Game_Display_Dead;
    Game_Display_Func[GAME_STT_PICK] = Game_Display_Pick;
    Game_Display_Func[GAME_STT_WIN] = Game_Display_Spawn_Win;
    Game_Display_Func[GAME_STT_END] = Game_Display_Begin_End;
    Game_Process_Func[GAME_STT_BEGIN] = Game_Process_Begin;
    Game_Process_Func[GAME_STT_SPAWN] = Game_Process_Spawn;
    Game_Process_Func[GAME_STT_PLAY] = Game_Process_Play;
    Game_Process_Func[GAME_STT_DEAD] = Game_Process_Dead;
    Game_Process_Func[GAME_STT_PICK] = Game_Process_Pick;
    Game_Process_Func[GAME_STT_WIN] = Game_Process_Win;
    Game_Process_Func[GAME_STT_END] = Game_Process_End;
    Collision_Tile_Func[CL_TILE_WALL] = Collision_Tile_Wall;
    Collision_Tile_Func[CL_TILE_NOTHING] = Collision_Tile_Nothing;
    Collision_Tile_Func[CL_TILE_TRAP] = Collision_Tile_Trap;
    Collision_Tile_Func[CL_TILE_DEST] = Collision_Tile_Dest;
}

void Collision_Tile() {
    x_Map = Player.x / TILE_SIZE;
    y_Map = Player.y / TILE_SIZE;
    Collision_Tile_Func[Map[y_Map][x_Map]]();
}
