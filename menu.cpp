void Menu_Display() {
    glClear(GL_COLOR_BUFFER_BIT);
    Map_Texture(&Img_Menu_BG);
    if (Menu_Offset >= WIDTH) {
        float l = 1 - Menu_Offset / MENU_BG_MAX_OFFSET;
        float r = l + 0.5f;
        glBegin(GL_POLYGON);
        glTexCoord2f(l, 0.0f);
        glVertex2f(0.0f, 0.0f);
        glTexCoord2f(r, 0.0f);
        glVertex2f(640.0f, 0.0f);
        glTexCoord2f(r, 1.0f);
        glVertex2f(640.0f, 480.0f);
        glTexCoord2f(l, 1.0f);
        glVertex2f(0.0f, 480.0f);
        glEnd();
    } else {
        float l = 1 - Menu_Offset / MENU_BG_MAX_OFFSET;
        float r = l - 0.5f;
        glBegin(GL_POLYGON);
        glTexCoord2f(l, 0.0f);
        glVertex2f(0.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(Menu_Offset, 0.0f);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(Menu_Offset, 480.0f);
        glTexCoord2f(l, 1.0f);
        glVertex2f(0.0f, 480.0f);
        glEnd();
        glBegin(GL_POLYGON);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(Menu_Offset, 0.0f);
        glTexCoord2f(r, 0.0f);
        glVertex2f(640.0f, 0.0f);
        glTexCoord2f(r, 1.0f);
        glVertex2f(640.0f, 480.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(Menu_Offset, 480.0f);
        glEnd();
    }
    Menu_Display_Func[Menu_Form_Stt]();
    glutSwapBuffers();
}

void Set_Rect_Btn_Lvl() {
    Rct_Menu_Btn_Lvl.Left = BTN_LVL_START_X + (Menu_Choice - 1) % BTN_LVL_MAX_PER_ROW * BTN_LVL_SIZE_FULL;
    Rct_Menu_Btn_Lvl.Bottom = BTN_LVL_START_Y - (Menu_Choice - 1) / BTN_LVL_MAX_PER_ROW * BTN_LVL_SIZE_FULL;
    Rct_Menu_Btn_Lvl.Right = Rct_Menu_Btn_Lvl.Left + BTN_LVL_SIZE;
    Rct_Menu_Btn_Lvl.Top = Rct_Menu_Btn_Lvl.Bottom + BTN_LVL_SIZE;
}

void Menu_Keyboard(unsigned char key, int x, int y) {
    if (key == 13) {
        if (Menu_Form_Stt == 0) {
            if (Menu_Choice == 0) {
                Menu_Form_Stt = 1;
                Menu_Choice = 1;
                Set_Rect_Btn_Lvl();
            } else {
            	// system("xdg-open https://www.youtube.com/channel/UCyRY53rs_lgyJ-p3PzZj0Og"); ubuntu
                system("start https://www.youtube.com/channel/UCyRY53rs_lgyJ-p3PzZj0Og");
            }
        } else {
            Game_Level = Menu_Choice;
            Menu_Active = 0;
            Reload();
            glutDisplayFunc(Game_Display);
            glutKeyboardFunc(Game_Keyboard);
            glutSpecialFunc(Game_Special);
            glutSpecialUpFunc(Game_Special_Up);
        }
    }
}

void Menu_Special(int key, int x, int y) {
    if (Menu_Form_Stt == 0) {
        switch (key) {
        case GLUT_KEY_LEFT:
        case GLUT_KEY_RIGHT:
        case GLUT_KEY_DOWN:
        case GLUT_KEY_UP:
            Menu_Choice = 1 - Menu_Choice;
            break;
        }
    } else {
        switch (key) {
        case GLUT_KEY_LEFT:
            Menu_Choice--;
            if (Menu_Choice % BTN_LVL_MAX_PER_ROW == 0) {
                Menu_Choice += BTN_LVL_MAX_PER_ROW;
                if (Menu_Choice > Menu_Max_Lvl)
                    Menu_Choice = Menu_Max_Lvl;
            }
            Set_Rect_Btn_Lvl();
            break;
        case GLUT_KEY_RIGHT:
            if (Menu_Choice % BTN_LVL_MAX_PER_ROW == 0)
                Menu_Choice -= BTN_LVL_MAX_PER_ROW - 1;
            else {
                Menu_Choice++;
                if (Menu_Choice > Menu_Max_Lvl)
                    Menu_Choice = Menu_Choice / BTN_LVL_MAX_PER_ROW * BTN_LVL_MAX_PER_ROW + 1;
            }
            Set_Rect_Btn_Lvl();
            break;
        case GLUT_KEY_DOWN:
            Menu_Choice += BTN_LVL_MAX_PER_ROW;
            if (Menu_Choice > Menu_Max_Lvl)
                Menu_Choice = (Menu_Choice - 1) % BTN_LVL_MAX_PER_ROW + 1;
            Set_Rect_Btn_Lvl();
            break;
        case GLUT_KEY_UP:
            Menu_Choice -= BTN_LVL_MAX_PER_ROW;
            if (Menu_Choice <= 0)
                Menu_Choice += BTN_LVL_MAX_REAL - (Menu_Choice + BTN_LVL_MAX_REAL - Menu_Max_Lvl + BTN_LVL_MAX_PER_ROW - 1) / BTN_LVL_MAX_PER_ROW * BTN_LVL_MAX_PER_ROW;
            Set_Rect_Btn_Lvl();
            break;
        }
    }
}
