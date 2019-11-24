# Gravity Duck

Game made by C++ OpenGL

### Content:
======
Setup:<br />
- Setup MinGW/G++<br />
- Open main.cpp or Gravity_Duck.dev if you have Dev C++<br />
- Only build main.cpp<br />

Linker:<br />
- Window: -lopengl32 -lglu32 -lglut32 -lSDL2 -lSDL2_mixer -mwindows -m32<br />
- Ubuntu: -lGL -lGLU -lglut -lSDL2 -lSDL2_mixer -lm<br />
Note:<br />
-lopengl32, -lglu32, -lglut32 & -lGL, -lGLU, -lglut for OpenGL<br />
-lSDL2, -lSDL2_mixer for Audio<br />
-lm for math.h<br />
-m32 for Win32 bit<br />
-mwindows for hide Console<br />

### Image:

![Image](https://lh3.googleusercontent.com/GHZhGiho1tM76SRA1NMiv6PaYWi7sNeT20V-wbPrQPa5NK20x-zn6Sa0BZVKjE35nK7lsPWQHPf9o8jNiVOI3q3XUmYbK5bOzz80RNS52Z-NWFUrALIJpozuPNtbfgxRYnosJ0N7MmTdwbelQ-91kMNegP3SrXVBfW4X-VXfFkcWWPYVXvhbm9jcAeu88JtdPO6ojhvABpQp7PJXUqUTcWnGRt-JWoACWqpcGqVMxrPPmGraoEAaGYK0GActeVn29KvCF8lVHanSvggFK9sZlVz1IyeYQ17HZkJJLmCwmZbefcEaoaIB98773jbTF3-zEfWNUUcaAbE-KWJERM8ZIZpbHwFCwQmLnL7mQTyHNiZxyRWPL9XU4pmwfi-x0XWiOMgdZtzfmIfvqrKr3wvRZgQPHe5IkZ-8EjwN6M_eujILu9Dg3KwUvbzpmmAClXw1CVQ1RmfWjGm3XzEqirpHIR47w_ijqM6HyEViT4JF-FcwKwnR6HPWVfG-z1bNQa4hHxKYfu4uzYLEdJ5O0sL2opbF87NAR6W7vqFK-bo8giNjFKNh3QpIY0yC26Fo9WkEhJ_89IgJ2aZgbC_w4s4uLdppv1fP_RGGoVpCjtUZlje0IZe_C34bHqor_bPNwbMGJT-C7qfUjo44gtYKjBoMUofoehyEB7Ytg27sddQwt3Xbyn0a-bLJSmPeN1TJmbpxLPXwQHbezQ6DTl3LK6XYGmqXuf5tFH4vZjBfP01mYdkXi7k=w640-h480-no)

### Type of Enemy:

- Enemy_Block_Horizontal<br />
- Enemy_Block_Vertical<br />
- Enemy_Worm<br />
- Enemy_Shooter<br />
- Enemy_Fly<br />
- Enemy_Fly_Chase<br />
- Enemy_Crusher<br />

### Map Structure:

Line 1: Appear of Duck: x real, y real, Gravity Direction<br />
Line 2: Destination Position: x, y, Gravity Direction<br />
From Line 3 to 17: Tiles of Map<br />
Next: Count of Switches<br />
Each switch has 1 Line: x, y, Rotation Direction (0 is ccw, 1 is cw)<br />
Next: Count of Enemies<br />
Each enemy has 1 Line: Type, x, y, ... follow type of Enemy<br />

### Note:

In Map_18<br />
Enemy_1: Move 0 Move_Max 81<br />
Enemy_2: Move 29 Move_Max 81<br />
Reason:<br />
Move_Max = Move_Max + 9 = 90 (Stt = 3, Stt Change Every 3 Time (Enemy_Time = 3) => +9 Offset For Move_Max)<br />
=> Move = Move_Max / 2 = 45 (Balance Bullet For 2 Enemies)<br />
But Enemy_1_X (17) > Enemy_2_X (15) = 2 * 32 = 64 (32 Is Size Of Tile)<br />
Offset_Bullet = 4 => Enemy_1_Latency = 64 / 4 = 16<br />
=> Enemy_1_Move = 45 - 16 = 29
<br />

### Conclusion:

Full 40 Maps - Thank to Armor Games<br />
Like original game 80%<br />
I fixed some places to make game more reasonable :D<br />

### Youtube:

https://www.youtube.com/watch?v=uzZv5hL9D2U

--- End ---
