void Player_Move_Left() {
    Player.x += Player.vx;
    A = (Player.x - 16) / 32;
    B = (Player.y - 10) / 32;
    C = (Player.y + 10) / 32;
    if (Map[B][A] == 0 || Map[C][A] == 0) {
        if (Gra_Mapping[Player.Gra]) {
            Player.vx = 0.0f;
            Switch_Reset_Touch();
        }
        Player.x = (A + 1) * 32 + 16;
    } else {
        if (Gra_Mapping[Player.Gra])
            Player.Is_Jump = 1;
        else
            Player.Is_Run = 1;
    }
    Player.Rct.Left = (int)Player.x - 16;
    Player.Rct.Right = (int)Player.x + 16;
}

void Player_Move_Right() {
    Player.x += Player.vx;
    A = (Player.x + 16) / 32;
    B = (Player.y - 10) / 32;
    C = (Player.y + 10) / 32;
    if (Map[B][A] == 0 || Map[C][A] == 0) {
        if (Gra_Mapping[Player.Gra]) {
            Player.vx = 0.0f;
            Switch_Reset_Touch();
        }
        Player.x = A * 32 - 16;
    } else {
        if (Gra_Mapping[Player.Gra])
            Player.Is_Jump = 1;
        else
            Player.Is_Run = 1;
    }
    Player.Rct.Left = (int)Player.x - 16;
    Player.Rct.Right = (int)Player.x + 16;
}

void Player_Move_Down() {
    Player.y += Player.vy;
    A = (Player.y - 16) / 32;
    B = (Player.x - 10) / 32;
    C = (Player.x + 10) / 32;
    if (Map[A][B] == 0 || Map[A][C] == 0) {
        if (!Gra_Mapping[Player.Gra]) {
            Player.vy = 0.0f;
            Switch_Reset_Touch();
        }
        Player.y = (A + 1) * 32 + 16;
    } else {
        if (!Gra_Mapping[Player.Gra])
            Player.Is_Jump = 1;
        else
            Player.Is_Run = 1;
    }
    Player.Rct.Bottom = (int)Player.y - 16;
    Player.Rct.Top = (int)Player.y + 16;
}

void Player_Move_Up() {
    Player.y += Player.vy;
    A = (Player.y + 16) / 32;
    B = (Player.x - 10) / 32;
    C = (Player.x + 10) / 32;
    if (Map[A][B] == 0 || Map[A][C] == 0) {
        if (!Gra_Mapping[Player.Gra]) {
            Player.vy = 0.0f;
            Switch_Reset_Touch();
        }
        Player.y = A * 32 - 16;
    } else {
        if (!Gra_Mapping[Player.Gra])
            Player.Is_Jump = 1;
        else
            Player.Is_Run = 1;
    }
    Player.Rct.Bottom = (int)Player.y - 16;
    Player.Rct.Top = (int)Player.y + 16;
}
