c_Switch::c_Switch() {
}

c_Switch::~c_Switch() {
}

void c_Switch::Set(float x, float y){
	this->x=x;
	this->y=y;
	Is_Touch = 0;
    Hitbox.Left=x-HITBOX_SWITCH;
    Hitbox.Right=x+HITBOX_SWITCH;
    Hitbox.Bottom=y-HITBOX_SWITCH;
    Hitbox.Top=y+HITBOX_SWITCH;
}

void Switch_Draw() {
    for (int i = 0; i < Switch_Count; i++) {
        glLoadIdentity();
        glTranslatef(Switch[i].x, Switch[i].y, 0.0f);
        glRotatef(Switch_Angle, 0.0f, 0.0f, 1.0f);
        Draw_Rect(&Rct_Switch);
    }
}

void Switch_Action() {
    for (int i = 0; i < Switch_Count; i++) {
        Ptr_Switch = &Switch[i];
        if (!Ptr_Switch->Is_Touch) {
            if (Player.x > Ptr_Switch->Hitbox.Left && Player.x < Ptr_Switch->Hitbox.Right &&
                Player.y > Ptr_Switch->Hitbox.Bottom && Player.y < Ptr_Switch->Hitbox.Top) {
                Ptr_Switch->Is_Touch = 1;
                Switch_List_Touch[Switch_Count_Touch] = i;
                Switch_Count_Touch++;
                Player.Gra = Gra_Next[Player.Gra];
                Player.gx = Gra_Offset[Player.Gra].x;
                Player.gy = Gra_Offset[Player.Gra].y;
                Player.vx = Player.vy = 0.0f;
                break;
            }
        }
    }
    Switch_Angle += 7.0f;
}

void Switch_Reset_Touch() {
    for (int i = 0; i < Switch_Count_Touch; i++)
        Switch[Switch_List_Touch[i]].Is_Touch = 0;
    Switch_Count_Touch = 0;
}