c_Bullet::c_Bullet(float x, float y, float vx, float vy) {
    this->x = x;
    this->y = y;
    this->vx = vx;
    this->vy = vy;
}

void c_Bullet::Collision() {
    if (Game_Stt == GAME_STT_PLAY && Player.x > Hitbox.Left && Player.x < Hitbox.Right && Player.y > Hitbox.Bottom && Player.y < Hitbox.Top) {
        Player.Set_Rct_Dead();
        Game_Stt = GAME_STT_DEAD;
        Mix_PlayChannel(-1, Sound_Dead, 0);
        Player.Alpha = 1.0f;
    }
}

c_Bullet_Shooter::c_Bullet_Shooter(float x, float y, float vx) : c_Bullet(x, y, vx, 0.0f) {
    Rct.Left = x - Bullet_Shooter_W / 2;
    Rct.Right = Rct.Left + Bullet_Shooter_W;
    Rct.Bottom = y - Bullet_Shooter_H / 2;
    Rct.Top = Rct.Bottom + Bullet_Shooter_H;
    Hitbox.Left = x - Bullet_Shooter_Hitbox_W;
    Hitbox.Right = x + Bullet_Shooter_Hitbox_W;
    Hitbox.Bottom = y - Bullet_Shooter_Hitbox_H;
    Hitbox.Top = y + Bullet_Shooter_Hitbox_H;
}

void c_Bullet_Shooter::Draw() {
    Map_Texture(&Img_Bullet_Shooter);
    Draw_Rect(&Rct);
}

void c_Bullet_Shooter::Action() {
    x += vx;
    Rct.Left = x - Bullet_Shooter_W / 2;
    Rct.Right = Rct.Left + Bullet_Shooter_W;
    Hitbox.Left = x - Bullet_Shooter_Hitbox_W;
    Hitbox.Right = x + Bullet_Shooter_Hitbox_W;
    Collision();
    Is_Alive = 1;
    if (Map[(int)y / TILE_SIZE][(int)x / TILE_SIZE] == CL_TILE_WALL)
        Is_Alive = 0;
}

c_Bullet_Fly::c_Bullet_Fly(float x, float y, float vx, float vy, float Angle) : c_Bullet(x, y, vx, vy) {
    this->Angle = Angle;
    Is_Explode = 0;
    Hitbox.Left = x - Bullet_Fly_Hitbox_W;
    Hitbox.Right = x + Bullet_Fly_Hitbox_W;
    Hitbox.Bottom = y - Bullet_Fly_Hitbox_H;
    Hitbox.Top = y + Bullet_Fly_Hitbox_H;
}

void c_Bullet_Fly::Draw() {
    if (!Is_Explode) {
        glTranslatef((int)x, (int)y, 0.0f);
        glRotatef(Angle, 0.0f, 0.0f, 1.0f);
        Map_Texture(&Img_Bullet_Fly);
        Draw_Rect(&Rct_Bullet_Fly);
        glLoadIdentity();
    } else {
        glColor4f(1.0f, 1.0f, 1.0f, Alpha_Explode);
        Map_Texture(&Img_Bullet_Fly_Explode);
        Draw_Rect(&Rct);
        glColor3f(1.0f, 1.0f, 1.0f);
    }
}

void c_Bullet_Fly::Action() {
    Is_Alive = 1;
    if (!Is_Explode) {
        x += vx;
        y += vy;
        Hitbox.Left = x - Bullet_Fly_Hitbox_W;
        Hitbox.Right = x + Bullet_Fly_Hitbox_W;
        Hitbox.Bottom = y - Bullet_Fly_Hitbox_H;
        Hitbox.Top = y + Bullet_Fly_Hitbox_H;
        Collision();
        if (Map[(int)y / TILE_SIZE][(int)x / TILE_SIZE] == CL_TILE_WALL) {
            Is_Explode = 1;
            Alpha_Explode = 1.0f;
            Rct.Left = (int)x - Bullet_Fly_Explode_W / 2.0f;
            Rct.Right = Rct.Left + Bullet_Fly_Explode_W;
            Rct.Bottom = (int)y - Bullet_Fly_Explode_H / 2.0f;
            Rct.Top = Rct.Bottom + Bullet_Fly_Explode_H;
        }
    } else {
        Alpha_Explode -= 0.05f;
        if (Alpha_Explode <= 0.0f)
            Is_Alive = 0;
    }
}

struct s_List_Bullet {
    c_Bullet *Bullet;
    s_List_Bullet *next;
    s_List_Bullet(c_Bullet *b) {
        Bullet = b;
        next = NULL;
    }
    ~s_List_Bullet() {
        delete Bullet;
    }
} *List_Bullet = NULL;

void List_Bullet_Add(c_Bullet *b) {
    s_List_Bullet *list = new s_List_Bullet(b);
    if (List_Bullet == NULL)
        List_Bullet = list;
    else {
        list->next = List_Bullet;
        List_Bullet = list;
    }
}