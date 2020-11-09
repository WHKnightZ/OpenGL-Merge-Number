#include "main.h"

#define WIDTH 424
#define HEIGHT 474
#define POS_X 420
#define POS_Y 160

#define MAX 5
#define MAX_BLOCK 25 // MAX * MAX
#define SIZE_CELL 60
#define MIDDLE_CELL 30 // SIZE_CELL / 2
#define SIZE_SPACE 6
#define SIZE_CELL_FULL 66
#define START_X 50
#define START_Y 100
#define FULL_X 374 // = START_X + MAX * SIZE_CELL_FULL - SIZE_SPACE
#define FULL_Y 424 // = START_Y + MAX * SIZE_CELL_FULL - SIZE_SPACE

#define INTERVAL 25
#define MAX_ANIMATION 12

#define SIZE_HON_RIPPLE 14
#define SIZE_VER_RIPPLE 60 // = SIZE_CELL
#define SIZE_HON_TAIL 30
#define SIZE_VER_TAIL 12   // = SIZE_CELL / 5
#define SIZE_HON_SQUARE 76 // SIZE_CELL_FULL + SIZE_CELL / 6
#define SIZE_VER_SQUARE 60 // = SIZE_CELL
#define DIS_TAIL 24        // = SIZE_CELL * 2 / 5
#define DONE_RIPPLE 12
#define DONE_TAIL 22
#define DONE_SQUARE 11
#define BFS_MAX_DEGREE 14
#define BFS_MAX_PER_DEGREE 14
#define BFS_MAX_LIST_MARK 25
#define OFFSET_JOINT_SQUARE 16 // = SIZE_CELL / 6 + SIZE_SPACE
#define SIZE_JOINT_SQUARE 26   // = SIZE_CELL * 2 / 6 + SIZE_SPACE
#define MAX_JOINT_SQUARE 24
#define NUM_FALL 8
#define OFFSET_FALL 8.25f // SIZE_CELL_FULL / NUM_FALL
#define MAX_RANDOM 5      // Easy 5 Hard 7
#define MAX_NUM 20
#define NUM_COLOR 160
#define MAX_LIFE 3
#define MAX_ZOOM 10
#define OFFSET_ZOOM 3.0f // SIZE_CELL / MAX_ZOOM / 2
#define MAX_STT_GAME_OVER 23
#define WAIT_GAME_OVER 60

typedef struct Struct_Animation {
    int Color;
    int Drt, Pos_Offset[3];
    char Done_Tail[3];
    float Left_Ripple, Right_Ripple, Bottom_Ripple, Top_Ripple;
    float Left_Square, Right_Square, Bottom_Square, Top_Square;
    float Left_Tail[3], Right_Tail[3], Bottom_Tail[3], Top_Tail[3];
} Struct_Animation;

typedef struct Image {
    GLubyte *img;
    GLuint w, h;
} Image;

typedef struct Struct_Pnt {
    char x, y;
} Struct_Pnt;

typedef struct Struct_BFS {
    char x, y;
    int Drt;
} Struct_BFS;

typedef struct Struct_BFS_Mark {
    char x, y;
} Struct_BFS_Mark;

typedef struct Struct_Joint_Square {
    float Left, Right, Bottom, Top;
} Struct_Joint_Square;

typedef struct Struct_Block_Fall {
    float Bottom, Top;
    char IsAlive, Value, x, y_New;
    int Count;
} Struct_Block_Fall;

typedef struct Struct_Block_Zoom {
    int Color;
    char x, y;
} Struct_Block_Zoom;

typedef struct Struct_Rectangle {
    float Left, Right, Bottom, Top;
} Struct_Rectangle;

typedef struct Struct_Region {
    int Left, Right, Bottom, Top;
} Struct_Region;

void (*Init_Pos_Ani[4])(Struct_Animation *, char *, char *);
void (*Action_Effect_Ani[4])(Struct_Animation *);
int (*Check_Equal[4])(char, char);
void (*Create_Joint_Square[4])(char *, char *);
void (*Display_Block_Make[2])();
void (*Make_Block_Appear[2])();
int (*Timer_Block_Make[2])();
void (*Func_Timer)(int);

Struct_Animation *Ptr_Ani;
Struct_Animation Animation[MAX_ANIMATION];
Struct_Pnt Def_Drt[4] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
Struct_BFS *Ptr_BFS;
Struct_BFS BFS_List[BFS_MAX_DEGREE][BFS_MAX_PER_DEGREE];
Struct_BFS_Mark BFS_List_Mark[BFS_MAX_LIST_MARK];
Struct_Joint_Square Joint_Square[MAX_JOINT_SQUARE];
Struct_Block_Fall *Ptr_Block_Fall;
Struct_Block_Fall Block_Fall[MAX_BLOCK];
Struct_Rectangle Rect_Logo;
Struct_Rectangle Rect_Zoom[MAX][MAX][MAX_ZOOM];
Struct_Rectangle Rect_Home;
Struct_Region Reg_Home;
Struct_Block_Zoom Block_Zoom[MAX_BLOCK];
Struct_Rectangle *Ptr_Rect;
Struct_Rectangle Rect_Game_Over[MAX_STT_GAME_OVER + 1];
Struct_Pnt List_Block_Zoom_Full[MAX_BLOCK];

Image Img_Logo, Img_Select_Yes, Img_Select_No, Img_Fall_Down, Img_Zoom_In, Img_Easy, Img_Hard;
Image Img_Select_Menu[2][2][2]; // Mode/Level - Button1/2 - No/Yes
Image Img_Block_Full, Img_Num[10];
Image Img_Title_Page[2];
Image Img_Block_Num[MAX_NUM], Img_Block_Ripple[4], Img_Block_Tail[4];
Image Img_Life, Img_Home, Img_Num_Score[10];
Image Img_Game_Over;

const int Offset_Done[3] = {13, 22, 17};
const int Loop_Next[] = {1, 2, 0};
const int Reverse_Drt[] = {1, 0, 3, 2};
const int BFS_Pos_Find[] = {2, 1, 3, 0, 4};
const float Select_Offset_Left[] = {9.0f, 0.0f}, Select_Offset_Right[] = {-9.0f, 0.0f}, Select_Offset_Bottom[] = {3.0f, 0.0f}, Select_Offset_Top[] = {-3.0f, 0.0f};

int IsRedisplay;
int Next_Number[MAX_NUM];
int Game_Over, Stt_Game_Over, Stt_Wait_Game_Over;
int Count_Block_Zoom_Full = 0;
int Status_Zoom = MAX_ZOOM;
int Menu_Page, Menu_Choice, Select_Choice[2];
int Increase_Life;
float Select_Left, Select_Right, Select_Bottom[2], Select_Top[2];
float Title_Page_Left, Title_Page_Bottom, Title_Page_Top, Title_Page_Right[2];
int Select_Region_Left, Select_Region_Right, Select_Region_Bottom[2], Select_Region_Top[2];
float Score_Bottom, Score_Top, Score_Right[6];
int Tmp_Color_0, Tmp_Color_1, Tmp_Color_2;
int Score[5], Life, Combo, Game_Mode;
float Life_Left[MAX_LIFE], Life_Right[MAX_LIFE], Life_Bottom, Life_Top;
int Count_Joint_Square;
int BFS_Count_List_Mark;
int BFS_Count_List[BFS_MAX_DEGREE];
char BFS_X, BFS_Y;
char BFS_Mark[MAX][MAX];
int BFS_Count, BFS_Degree, BFS_Degree_Save, BFS_Result, xx, yy;
char X_Current, Y_Current;
int Pos_Effect = DONE_TAIL, Count_Animation;
char Current_Value;
float Offset_Ripple[DONE_RIPPLE] = {9.0f};
float Offset_Tail[3][DONE_TAIL] = {{8.0f}, {6.0f}, {7.0f}};
float Pos_Left[MAX];
float Pos_Right[MAX];
float Pos_Bottom[MAX];
float Pos_Top[MAX];
int Mark_Column_Fall[MAX], Mark_Column_Fall_Full[MAX];
int Pos_Block_Fall, Count_Done_Block_Fall, Count_Block_Make = 0, Column_Need;
int Tmp_Int, Tmp_Int_2;
int i, j, k, ii, jj, kk, ll, mm, nn;
char Mark_Block_Zoom_Full[MAX][MAX];

const float Color_Block[MAX_NUM][3] = {
    {0.902f, 0.675f, 0.706f},
    {0.655f, 0.847f, 0.925f},
    {0.757f, 0.878f, 0.675f},
    {0.918f, 0.906f, 0.580f},
    {0.890f, 0.725f, 0.914f},
    {0.953f, 0.443f, 0.392f},
    {0.553f, 0.592f, 0.847f},
    {0.502f, 0.890f, 0.839f},
    {0.506f, 0.824f, 0.357f},
    {0.784f, 0.604f, 0.471f},
    {0.902f, 0.675f, 0.706f},
    {0.655f, 0.847f, 0.925f},
    {0.757f, 0.878f, 0.675f},
    {0.918f, 0.906f, 0.580f},
    {0.890f, 0.725f, 0.914f},
    {0.953f, 0.443f, 0.392f},
    {0.553f, 0.592f, 0.847f},
    {0.502f, 0.890f, 0.839f},
    {0.506f, 0.824f, 0.357f},
    {0.784f, 0.604f, 0.471f}};

char Map[MAX][MAX];

char Map_1[MAX][MAX] = {
    {1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1},
    {1, 1, 0, 1, 1},
    {1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1}};

char Map_2[MAX][MAX] = {
    {1, 3, 2, 3, 1},
    {1, 0, 1, 0, 1},
    {0, 2, 1, 2, 0},
    {4, 1, 0, 1, 4},
    {4, 2, 1, 2, 4}};

void Func_Timer_Null(int value) {}

void Load_Texture(Image *img, const char *path) {
    Load_Png(&img->img, &img->w, &img->h, path);
}

void Map_Texture(Image *img) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->w, img->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->img);
}

void Increase_Score(int score) {
    int n = 0;
    Score[n] += score;
    while (Score[n] >= 10) {
        Score[n + 1] += Score[n] / 10;
        Score[n] %= 10;
        n++;
    }
}

void Set_Image(Image *Img_Dest, Image *Img_Source) {
    int i;
    Img_Dest->w = Img_Source->w;
    Img_Dest->h = Img_Source->h;
    int Size = Img_Dest->w * Img_Dest->h * 4;
    Img_Dest->img = malloc(Size);
    GLubyte *ptr = Img_Dest->img, *ptr2 = Img_Source->img;
    for (i = 0; i < Size; i++)
        *(ptr++) = *(ptr2++);
}

void Mix_Image(Image *Img_BG, Image *Img_FG, int x, int y) {
    int tmp, i, j;
    int w_BG = Img_BG->w;
    int w_FG = Img_FG->w, h_FG = Img_FG->h;
    unsigned char *img1 = Img_BG->img, *img2 = Img_FG->img;
    img1 += w_BG * 4 * y + x * 4;
    for (i = 0; i < h_FG; i++) {
        for (j = 0; j < w_FG; j++) {
            tmp = img1[3];
            img1[3] = img1[3] + img2[3] - img1[3] * img2[3] / 255;
            if (img1[3] != 0) {
                img1[0] = (img2[0] * img2[3] + img1[0] * tmp * (255 - img2[3]) / 255) / img1[3];
                img1[1] = (img2[1] * img2[3] + img1[1] * tmp * (255 - img2[3]) / 255) / img1[3];
                img1[2] = (img2[2] * img2[3] + img1[2] * tmp * (255 - img2[3]) / 255) / img1[3];
            }
            img1 += 4;
            img2 += 4;
        }
        img1 += (w_BG - w_FG) * 4;
    }
}

void Mix_Image_Color(Image *Img_BG, Image *Img_FG, int x, int y, unsigned char New_Color) {
    int tmp, i, j;
    int w_BG = Img_BG->w;
    int w_FG = Img_FG->w, h_FG = Img_FG->h;
    unsigned char *img1 = Img_BG->img, *img2 = Img_FG->img;
    img1 += w_BG * 4 * y + x * 4;
    for (i = 0; i < h_FG; i++) {
        for (j = 0; j < w_FG; j++) {
            tmp = img1[3];
            img1[3] = img1[3] + img2[3] - img1[3] * img2[3] / 255;
            if (img1[3] != 0) {
                img1[0] = (New_Color * img2[3] + img1[0] * tmp * (255 - img2[3]) / 255) / img1[3];
                img1[1] = (New_Color * img2[3] + img1[1] * tmp * (255 - img2[3]) / 255) / img1[3];
                img1[2] = (New_Color * img2[3] + img1[2] * tmp * (255 - img2[3]) / 255) / img1[3];
            }
            img1 += 4;
            img2 += 4;
        }
        img1 += (w_BG - w_FG) * 4;
    }
}

void Init_Menu_Image() {
    int i, j, k;
    unsigned char *ptr;
    for (i = 0; i < 2; i++)
        for (j = 0; j < 2; j++) {
            Set_Image(&Img_Select_Menu[i][j][0], &Img_Select_No);
            Set_Image(&Img_Select_Menu[i][j][1], &Img_Select_Yes);
        }
    for (i = 0; i < 2; i++) {
        Mix_Image(&Img_Select_Menu[0][0][i], &Img_Fall_Down, 150 - Img_Fall_Down.w / 2, 50 - Img_Fall_Down.h / 2);
        Mix_Image(&Img_Select_Menu[0][1][i], &Img_Zoom_In, 150 - Img_Zoom_In.w / 2, 50 - Img_Zoom_In.h / 2);
        Mix_Image(&Img_Select_Menu[1][0][i], &Img_Easy, 150 - Img_Easy.w / 2, 50 - Img_Easy.h / 2);
        Mix_Image(&Img_Select_Menu[1][1][i], &Img_Hard, 150 - Img_Hard.w / 2, 50 - Img_Hard.h / 2);
    }
    for (i = 0; i < 2; i++)
        for (j = 0; j < 2; j++) {
            ptr = Img_Select_Menu[i][j][0].img + 3;
            for (k = 0; k < 30000; k++) {
                *ptr *= 0.8f;
                ptr += 4;
            }
        }
    free(Img_Select_No.img);
    free(Img_Select_Yes.img);
    free(Img_Fall_Down.img);
    free(Img_Zoom_In.img);
    free(Img_Easy.img);
    free(Img_Hard.img);
    Title_Page_Left = 120.0f;
    Title_Page_Bottom = 215.0f;
    Title_Page_Top = Title_Page_Bottom + Img_Title_Page[0].h;
    Title_Page_Right[0] = Title_Page_Left + Img_Title_Page[0].w;
    Title_Page_Right[1] = Title_Page_Left + Img_Title_Page[1].w;
    Rect_Logo.Left = (WIDTH - Img_Logo.w) / 2;
    Rect_Logo.Right = Rect_Logo.Left + Img_Logo.w;
    Rect_Logo.Bottom = 50.0f;
    Rect_Logo.Top = Rect_Logo.Bottom + Img_Logo.h;
    Select_Left = (WIDTH - Img_Select_Menu[0][0][0].w) / 2;
    Select_Right = Select_Left + Img_Select_Menu[0][0][0].w;
    Select_Bottom[0] = 250.0f;
    Select_Top[0] = Select_Bottom[0] + Img_Select_Menu[0][0][0].h;
    Select_Bottom[1] = 330.0f;
    Select_Top[1] = Select_Bottom[1] + Img_Select_Menu[0][0][0].h;
    Select_Region_Left = Select_Left;
    Select_Region_Right = Select_Right;
    Select_Region_Bottom[0] = Select_Bottom[0] + 10;
    Select_Region_Top[0] = Select_Top[0] - 10;
    Select_Region_Bottom[1] = Select_Bottom[1] + 10;
    Select_Region_Top[1] = Select_Top[1] - 10;
}

void Init_Game_Image() {
    for (i = 0; i < MAX_LIFE; i++) {
        Life_Left[i] = 60.0f + 50 * i;
        Life_Right[i] = Life_Left[i] + Img_Life.w;
    }
    Life_Bottom = 42.0f;
    Life_Top = Life_Bottom + Img_Life.h;
    Score_Right[0] = 318.0f;
    Score_Bottom = 49.0f;
    Score_Top = Score_Bottom + Img_Num[0].h;
    Reg_Home.Left = Rect_Home.Left = 337.0f;
    Reg_Home.Bottom = Rect_Home.Bottom = 36.0f;
    Reg_Home.Right = Rect_Home.Right = Rect_Home.Left + Img_Home.w;
    Reg_Home.Top = Rect_Home.Top = Rect_Home.Bottom + Img_Home.h;
    float Offset_Hon[14] = {9};
    float Offset_Ver[14] = {3};
    int n = 0;
    for (i = 0; i < 13; i++) {
        Offset_Hon[i + 1] = Offset_Hon[i] * -0.8f;
        Offset_Ver[i + 1] = Offset_Ver[i] * -0.8f;
    }
    for (i = 0; i < 14; i++) {
        Offset_Hon[i] = (int)Offset_Hon[i];
        Offset_Ver[i] = (int)Offset_Ver[i];
    }
    Rect_Game_Over[9].Left = (WIDTH - Img_Game_Over.w) / 2;
    Rect_Game_Over[9].Bottom = 180.0f;
    Rect_Game_Over[9].Right = Rect_Game_Over[9].Left + Img_Game_Over.w;
    Rect_Game_Over[9].Top = Rect_Game_Over[9].Bottom + Img_Game_Over.h;
    int Offset_Zoom_Hon = Img_Game_Over.w / 10.0f, Offset_Zoom_Ver = Img_Game_Over.h / 10.0f;
    for (i = 8; i >= 0; i--) {
        Rect_Game_Over[i].Left = Rect_Game_Over[i + 1].Left - Offset_Zoom_Hon;
        Rect_Game_Over[i].Right = Rect_Game_Over[i + 1].Right + Offset_Zoom_Hon;
        Rect_Game_Over[i].Bottom = Rect_Game_Over[i + 1].Bottom - Offset_Zoom_Ver;
        Rect_Game_Over[i].Top = Rect_Game_Over[i + 1].Top + Offset_Zoom_Ver;
    }
    for (i = 10; i < MAX_STT_GAME_OVER + 1; i++) {
        Rect_Game_Over[i].Left = Rect_Game_Over[9].Left + Offset_Hon[i - 10];
        Rect_Game_Over[i].Right = Rect_Game_Over[9].Right + Offset_Hon[i - 10];
        Rect_Game_Over[i].Bottom = Rect_Game_Over[9].Bottom + Offset_Ver[i - 10];
        Rect_Game_Over[i].Top = Rect_Game_Over[9].Top + Offset_Ver[i - 10];
    }
}

void Init_Zoom_Position() {
    int i, j, k;
    for (i = 0; i < MAX; i++)
        for (j = 0; j < MAX; j++) {
            Rect_Zoom[i][j][0].Left = START_X + SIZE_CELL_FULL * j + MIDDLE_CELL - OFFSET_ZOOM;
            Rect_Zoom[i][j][0].Right = START_X + SIZE_CELL_FULL * j + MIDDLE_CELL + OFFSET_ZOOM;
            Rect_Zoom[i][j][0].Bottom = START_Y + SIZE_CELL_FULL * i + MIDDLE_CELL - OFFSET_ZOOM;
            Rect_Zoom[i][j][0].Top = START_Y + SIZE_CELL_FULL * i + MIDDLE_CELL + OFFSET_ZOOM;
            for (k = 0; k < MAX_ZOOM - 1; k++) {
                Rect_Zoom[i][j][k + 1].Left = Rect_Zoom[i][j][k].Left - OFFSET_ZOOM;
                Rect_Zoom[i][j][k + 1].Right = Rect_Zoom[i][j][k].Right + OFFSET_ZOOM;
                Rect_Zoom[i][j][k + 1].Bottom = Rect_Zoom[i][j][k].Bottom - OFFSET_ZOOM;
                Rect_Zoom[i][j][k + 1].Top = Rect_Zoom[i][j][k].Top + OFFSET_ZOOM;
            }
        }
}

int Check_Done_Ripple() {
    return Pos_Effect >= DONE_RIPPLE;
}

int Check_Done_Square() {
    return Pos_Effect >= DONE_SQUARE;
}

int Check_Done_Effect() {
    return Pos_Effect >= DONE_TAIL;
}

int Check_Equal_Up(char x, char y) {
    y--;
    if (y > -1) {
        if (!BFS_Mark[y][x]) {
            BFS_Mark[y][x] = 1;
            BFS_List_Mark[BFS_Count_List_Mark].x = x;
            BFS_List_Mark[BFS_Count_List_Mark].y = y;
            BFS_Count_List_Mark++;
            if (Map[y][x] == Current_Value)
                return 1;
        }
    }
    return 0;
}
int Check_Equal_Down(char x, char y) {
    y++;
    if (y < MAX) {
        if (!BFS_Mark[y][x]) {
            BFS_Mark[y][x] = 1;
            BFS_List_Mark[BFS_Count_List_Mark].x = x;
            BFS_List_Mark[BFS_Count_List_Mark].y = y;
            BFS_Count_List_Mark++;
            if (Map[y][x] == Current_Value)
                return 1;
        }
    }
    return 0;
}
int Check_Equal_Left(char x, char y) {
    x--;
    if (x > -1) {
        if (!BFS_Mark[y][x]) {
            BFS_Mark[y][x] = 1;
            BFS_List_Mark[BFS_Count_List_Mark].x = x;
            BFS_List_Mark[BFS_Count_List_Mark].y = y;
            BFS_Count_List_Mark++;
            if (Map[y][x] == Current_Value)
                return 1;
        }
    }
    return 0;
}
int Check_Equal_Right(char x, char y) {
    x++;
    if (x < MAX) {
        if (!BFS_Mark[y][x]) {
            BFS_Mark[y][x] = 1;
            BFS_List_Mark[BFS_Count_List_Mark].x = x;
            BFS_List_Mark[BFS_Count_List_Mark].y = y;
            BFS_Count_List_Mark++;
            if (Map[y][x] == Current_Value)
                return 1;
        }
    }
    return 0;
}

void Init_Pos_LRBT() {
    for (i = 0; i < MAX; i++) {
        Pos_Left[i] = START_X + i * SIZE_CELL_FULL;
        Pos_Right[i] = Pos_Left[i] + SIZE_CELL;
        Pos_Bottom[i] = START_Y + i * SIZE_CELL_FULL;
        Pos_Top[i] = Pos_Bottom[i] + SIZE_CELL;
    }
}

void Init_Pos_Offset() {
    for (i = 0; i < MAX_ANIMATION; i++) {
        Animation[i].Pos_Offset[0] = rand() % 3;
        do {
            Animation[i].Pos_Offset[1] = rand() % 3;
        } while (Animation[i].Pos_Offset[0] == Animation[i].Pos_Offset[1]);
        Animation[i].Pos_Offset[2] = 3 - (Animation[i].Pos_Offset[0] + Animation[i].Pos_Offset[1]);
    }
    for (i = 1; i < DONE_RIPPLE; i++)
        Offset_Ripple[i] = Offset_Ripple[i - 1] * 0.96f;
    for (i = 1; i < DONE_RIPPLE; i++)
        Offset_Ripple[i] = (int)Offset_Ripple[i];
    for (i = 1; i < DONE_TAIL; i++)
        for (j = 0; j < 3; j++)
            Offset_Tail[j][i] = Offset_Tail[j][i - 1] * 0.96f;
    for (i = 1; i < DONE_TAIL; i++)
        for (j = 0; j < 3; j++)
            Offset_Tail[j][i] = (int)Offset_Tail[j][i];
}

void Init_Pos_Ani_Up(Struct_Animation *ptr, char *x, char *y) {
    ptr->Left_Ripple = ptr->Left_Square = ptr->Left_Tail[0] = Pos_Left[*x];
    ptr->Right_Ripple = ptr->Right_Square = ptr->Right_Tail[2] = Pos_Right[*x];
    ptr->Top_Ripple = ptr->Top_Tail[0] = ptr->Top_Tail[1] = ptr->Top_Tail[2] = Pos_Top[*y];
    ptr->Bottom_Ripple = ptr->Top_Ripple - SIZE_HON_RIPPLE;
    ptr->Top_Square = ptr->Bottom_Ripple;
    ptr->Bottom_Square = ptr->Top_Ripple - SIZE_HON_SQUARE;
    ptr->Bottom_Tail[0] = ptr->Bottom_Tail[1] = ptr->Bottom_Tail[2] = ptr->Top_Ripple - SIZE_HON_TAIL;
    ptr->Left_Tail[1] = ptr->Left_Tail[0] + DIS_TAIL;
    ptr->Left_Tail[2] = ptr->Left_Tail[1] + DIS_TAIL;
    ptr->Right_Tail[0] = ptr->Left_Tail[0] + SIZE_VER_TAIL;
    ptr->Right_Tail[1] = ptr->Left_Tail[1] + SIZE_VER_TAIL;
}
void Init_Pos_Ani_Down(Struct_Animation *ptr, char *x, char *y) {
    ptr->Left_Ripple = ptr->Left_Square = ptr->Left_Tail[0] = Pos_Left[*x];
    ptr->Right_Ripple = ptr->Right_Square = ptr->Right_Tail[2] = Pos_Right[*x];
    ptr->Bottom_Ripple = ptr->Bottom_Tail[0] = ptr->Bottom_Tail[1] = ptr->Bottom_Tail[2] = Pos_Bottom[*y];
    ptr->Top_Ripple = ptr->Bottom_Ripple + SIZE_HON_RIPPLE;
    ptr->Bottom_Square = ptr->Top_Ripple;
    ptr->Top_Square = ptr->Bottom_Ripple + SIZE_HON_SQUARE;
    ptr->Top_Tail[0] = ptr->Top_Tail[1] = ptr->Top_Tail[2] = ptr->Top_Ripple + SIZE_HON_TAIL;
    ptr->Left_Tail[1] = ptr->Left_Tail[0] + DIS_TAIL;
    ptr->Left_Tail[2] = ptr->Left_Tail[1] + DIS_TAIL;
    ptr->Right_Tail[0] = ptr->Left_Tail[0] + SIZE_VER_TAIL;
    ptr->Right_Tail[1] = ptr->Left_Tail[1] + SIZE_VER_TAIL;
}
void Init_Pos_Ani_Left(Struct_Animation *ptr, char *x, char *y) {
    ptr->Bottom_Ripple = ptr->Bottom_Square = ptr->Bottom_Tail[0] = Pos_Bottom[*y];
    ptr->Top_Ripple = ptr->Top_Square = ptr->Top_Tail[2] = Pos_Top[*y];
    ptr->Right_Ripple = ptr->Right_Tail[0] = ptr->Right_Tail[1] = ptr->Right_Tail[2] = Pos_Right[*x];
    ptr->Left_Ripple = ptr->Right_Ripple - SIZE_HON_RIPPLE;
    ptr->Right_Square = ptr->Left_Ripple;
    ptr->Left_Square = ptr->Right_Ripple - SIZE_HON_SQUARE;
    ptr->Left_Tail[0] = ptr->Left_Tail[1] = ptr->Left_Tail[2] = ptr->Right_Ripple - SIZE_HON_TAIL;
    ptr->Bottom_Tail[1] = ptr->Bottom_Tail[0] + DIS_TAIL;
    ptr->Bottom_Tail[2] = ptr->Bottom_Tail[1] + DIS_TAIL;
    ptr->Top_Tail[0] = ptr->Bottom_Tail[0] + SIZE_VER_TAIL;
    ptr->Top_Tail[1] = ptr->Bottom_Tail[1] + SIZE_VER_TAIL;
}
void Init_Pos_Ani_Right(Struct_Animation *ptr, char *x, char *y) {
    ptr->Bottom_Ripple = ptr->Bottom_Square = ptr->Bottom_Tail[0] = Pos_Bottom[*y];
    ptr->Top_Ripple = ptr->Top_Square = ptr->Top_Tail[2] = Pos_Top[*y];
    ptr->Left_Ripple = ptr->Left_Tail[0] = ptr->Left_Tail[1] = ptr->Left_Tail[2] = Pos_Left[*x];
    ptr->Right_Ripple = ptr->Left_Ripple + SIZE_HON_RIPPLE;
    ptr->Left_Square = ptr->Right_Ripple;
    ptr->Right_Square = ptr->Left_Ripple + SIZE_HON_SQUARE;
    ptr->Right_Tail[0] = ptr->Right_Tail[1] = ptr->Right_Tail[2] = ptr->Left_Ripple + SIZE_HON_TAIL;
    ptr->Bottom_Tail[1] = ptr->Bottom_Tail[0] + DIS_TAIL;
    ptr->Bottom_Tail[2] = ptr->Bottom_Tail[1] + DIS_TAIL;
    ptr->Top_Tail[0] = ptr->Bottom_Tail[0] + SIZE_VER_TAIL;
    ptr->Top_Tail[1] = ptr->Bottom_Tail[1] + SIZE_VER_TAIL;
}

void Action_Effect_Ani_Up(Struct_Animation *ptr) {
    if (!Check_Done_Ripple()) {
        ptr->Bottom_Ripple -= Offset_Ripple[Pos_Effect];
        ptr->Top_Ripple -= Offset_Ripple[Pos_Effect];
        ptr->Top_Square = ptr->Bottom_Ripple;
    }
    for (ii = 0; ii < 3; ii++) {
        if (!ptr->Done_Tail[ii]) {
            ptr->Bottom_Tail[ii] -= Offset_Tail[ptr->Pos_Offset[ii]][Pos_Effect];
            ptr->Top_Tail[ii] = ptr->Bottom_Tail[ii] + SIZE_HON_TAIL;
            if (Pos_Effect == Offset_Done[ptr->Pos_Offset[ii]])
                ptr->Done_Tail[ii] = 1;
        }
    }
}
void Action_Effect_Ani_Down(Struct_Animation *ptr) {
    if (!Check_Done_Ripple()) {
        ptr->Bottom_Ripple += Offset_Ripple[Pos_Effect];
        ptr->Top_Ripple += Offset_Ripple[Pos_Effect];
        ptr->Bottom_Square = ptr->Top_Ripple;
    }
    for (ii = 0; ii < 3; ii++) {
        if (!ptr->Done_Tail[ii]) {
            ptr->Bottom_Tail[ii] += Offset_Tail[ptr->Pos_Offset[ii]][Pos_Effect];
            ptr->Top_Tail[ii] = ptr->Bottom_Tail[ii] + SIZE_HON_TAIL;
            if (Pos_Effect == Offset_Done[ptr->Pos_Offset[ii]])
                ptr->Done_Tail[ii] = 1;
        }
    }
}
void Action_Effect_Ani_Left(Struct_Animation *ptr) {
    if (!Check_Done_Ripple()) {
        ptr->Left_Ripple -= Offset_Ripple[Pos_Effect];
        ptr->Right_Ripple -= Offset_Ripple[Pos_Effect];
        ptr->Right_Square = ptr->Left_Ripple;
    }
    for (ii = 0; ii < 3; ii++) {
        if (!ptr->Done_Tail[ii]) {
            ptr->Left_Tail[ii] -= Offset_Tail[ptr->Pos_Offset[ii]][Pos_Effect];
            ptr->Right_Tail[ii] = ptr->Left_Tail[ii] + SIZE_HON_TAIL;
            if (Pos_Effect == Offset_Done[ptr->Pos_Offset[ii]])
                ptr->Done_Tail[ii] = 1;
        }
    }
}
void Action_Effect_Ani_Right(Struct_Animation *ptr) {
    if (!Check_Done_Ripple()) {
        ptr->Left_Ripple += Offset_Ripple[Pos_Effect];
        ptr->Right_Ripple += Offset_Ripple[Pos_Effect];
        ptr->Left_Square = ptr->Right_Ripple;
    }
    for (ii = 0; ii < 3; ii++) {
        if (!ptr->Done_Tail[ii]) {
            ptr->Left_Tail[ii] += Offset_Tail[ptr->Pos_Offset[ii]][Pos_Effect];
            ptr->Right_Tail[ii] = ptr->Left_Tail[ii] + SIZE_HON_TAIL;
            if (Pos_Effect == Offset_Done[ptr->Pos_Offset[ii]])
                ptr->Done_Tail[ii] = 1;
        }
    }
}

void Draw_Effect_Ani(Struct_Animation *ptr) {
    if (!Check_Done_Ripple()) {
        Map_Texture(&Img_Block_Ripple[ptr->Drt]);
        glBegin(GL_POLYGON);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(ptr->Left_Ripple, ptr->Bottom_Ripple);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(ptr->Right_Ripple, ptr->Bottom_Ripple);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(ptr->Right_Ripple, ptr->Top_Ripple);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(ptr->Left_Ripple, ptr->Top_Ripple);
        glEnd();
    }
    if (!Check_Done_Square()) {
        glDisable(GL_TEXTURE_2D);
        glBegin(GL_POLYGON);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(ptr->Left_Square, ptr->Bottom_Square);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(ptr->Right_Square, ptr->Bottom_Square);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(ptr->Right_Square, ptr->Top_Square);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(ptr->Left_Square, ptr->Top_Square);
        glEnd();
        glEnable(GL_TEXTURE_2D);
    }
    Map_Texture(&Img_Block_Tail[ptr->Drt]);
    for (jj = 0; jj < 3; jj++) {
        if (!ptr->Done_Tail[jj]) {
            glBegin(GL_POLYGON);
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(ptr->Left_Tail[jj], ptr->Bottom_Tail[jj]);
            glTexCoord2f(1.0f, 0.0f);
            glVertex2f(ptr->Right_Tail[jj], ptr->Bottom_Tail[jj]);
            glTexCoord2f(1.0f, 1.0f);
            glVertex2f(ptr->Right_Tail[jj], ptr->Top_Tail[jj]);
            glTexCoord2f(0.0f, 1.0f);
            glVertex2f(ptr->Left_Tail[jj], ptr->Top_Tail[jj]);
            glEnd();
        }
    }
}

void Create_Animation(char x, char y, int Drt) {
    Map[y][x] = -1;
    Ptr_Ani = &Animation[Count_Animation];
    Ptr_Ani->Drt = Drt;
    Ptr_Ani->Color = Map[y + Def_Drt[Drt].y][x + Def_Drt[Drt].x];
    Ptr_Ani->Done_Tail[0] = Ptr_Ani->Done_Tail[1] = Ptr_Ani->Done_Tail[2] = 0;
    Tmp_Int = rand() % 3;
    Tmp_Int_2 = Ptr_Ani->Pos_Offset[Tmp_Int];
    Ptr_Ani->Pos_Offset[Tmp_Int] = Ptr_Ani->Pos_Offset[Loop_Next[Tmp_Int]];
    Ptr_Ani->Pos_Offset[Loop_Next[Tmp_Int]] = Tmp_Int_2;
    Init_Pos_Ani[Drt](Ptr_Ani, &x, &y);
    Count_Animation++;
}

void Create_Joint_Square_Up(char *x, char *y) {
    Joint_Square[Count_Joint_Square].Left = Pos_Left[*x];
    Joint_Square[Count_Joint_Square].Right = Pos_Right[*x];
    Joint_Square[Count_Joint_Square].Bottom = Pos_Bottom[*y] - OFFSET_JOINT_SQUARE;
    Joint_Square[Count_Joint_Square].Top = Joint_Square[Count_Joint_Square].Bottom + SIZE_JOINT_SQUARE;
    Count_Joint_Square++;
}
void Create_Joint_Square_Down(char *x, char *y) {
    Joint_Square[Count_Joint_Square].Left = Pos_Left[*x];
    Joint_Square[Count_Joint_Square].Right = Pos_Right[*x];
    Joint_Square[Count_Joint_Square].Top = Pos_Top[*y] + OFFSET_JOINT_SQUARE;
    Joint_Square[Count_Joint_Square].Bottom = Joint_Square[Count_Joint_Square].Top - SIZE_JOINT_SQUARE;
    Count_Joint_Square++;
}
void Create_Joint_Square_Left(char *x, char *y) {
    Joint_Square[Count_Joint_Square].Bottom = Pos_Bottom[*y];
    Joint_Square[Count_Joint_Square].Top = Pos_Top[*y];
    Joint_Square[Count_Joint_Square].Left = Pos_Left[*x] - OFFSET_JOINT_SQUARE;
    Joint_Square[Count_Joint_Square].Right = Joint_Square[Count_Joint_Square].Left + SIZE_JOINT_SQUARE;
    Count_Joint_Square++;
}
void Create_Joint_Square_Right(char *x, char *y) {
    Joint_Square[Count_Joint_Square].Bottom = Pos_Bottom[*y];
    Joint_Square[Count_Joint_Square].Top = Pos_Top[*y];
    Joint_Square[Count_Joint_Square].Right = Pos_Right[*x] + OFFSET_JOINT_SQUARE;
    Joint_Square[Count_Joint_Square].Left = Joint_Square[Count_Joint_Square].Right - SIZE_JOINT_SQUARE;
    Count_Joint_Square++;
}

void Create_Block_Fall(char x, char y, char Value, int Count) {
    Ptr_Block_Fall = &Block_Fall[Count_Block_Make];
    Ptr_Block_Fall->x = x;
    Ptr_Block_Fall->Bottom = y * SIZE_CELL_FULL + START_Y;
    Ptr_Block_Fall->Top = Block_Fall[Count_Block_Make].Bottom + SIZE_CELL;
    Ptr_Block_Fall->IsAlive = 1;
    Ptr_Block_Fall->Value = Value;
    Ptr_Block_Fall->y_New = y + Count;
    Ptr_Block_Fall->Count = Count * NUM_FALL;
    Count_Block_Make++;
}

void Draw_Block_Fall() {
    for (k = 0; k < Count_Block_Make; k++) {
        Ptr_Block_Fall = &Block_Fall[k];
        if (Ptr_Block_Fall->IsAlive) {
            Map_Texture(&Img_Block_Num[Ptr_Block_Fall->Value]);
            glColor3fv(Color_Block[Ptr_Block_Fall->Value]);
            glBegin(GL_POLYGON);
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(Pos_Left[Ptr_Block_Fall->x], Ptr_Block_Fall->Bottom);
            glTexCoord2f(1.0f, 0.0f);
            glVertex2f(Pos_Right[Ptr_Block_Fall->x], Ptr_Block_Fall->Bottom);
            glTexCoord2f(1.0f, 1.0f);
            glVertex2f(Pos_Right[Ptr_Block_Fall->x], Ptr_Block_Fall->Top);
            glTexCoord2f(0.0f, 1.0f);
            glVertex2f(Pos_Left[Ptr_Block_Fall->x], Ptr_Block_Fall->Top);
            glEnd();
        }
    }
}

void Draw_Block_Zoom() {
    for (k = 0; k < Count_Block_Make; k++) {
        Ptr_Rect = &Rect_Zoom[Block_Zoom[k].y][Block_Zoom[k].x][Status_Zoom];
        glColor3fv(Color_Block[Block_Zoom[k].Color]);
        Map_Texture(&Img_Block_Num[Block_Zoom[k].Color]);
        glBegin(GL_POLYGON);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(Ptr_Rect->Left, Ptr_Rect->Bottom);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(Ptr_Rect->Right, Ptr_Rect->Bottom);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(Ptr_Rect->Right, Ptr_Rect->Top);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(Ptr_Rect->Left, Ptr_Rect->Top);
        glEnd();
    }
}

int BFS_Init(char x, char y) {
    Current_Value = Map[y][x];
    BFS_Degree = 0;
    BFS_Count = 0;
    BFS_Count_List[BFS_Degree] = 0;
    BFS_List_Mark[0].x = x;
    BFS_List_Mark[0].y = y;
    BFS_Count_List_Mark = 1;
    BFS_Mark[y][x] = 1;
    for (kk = 0; kk < 4; kk++) {
        if (Check_Equal[kk](x, y)) {
            Ptr_BFS = &BFS_List[BFS_Degree][BFS_Count_List[BFS_Degree]];
            Ptr_BFS->x = x + Def_Drt[kk].x;
            Ptr_BFS->y = y + Def_Drt[kk].y;
            Ptr_BFS->Drt = Reverse_Drt[kk];
            BFS_Count_List[BFS_Degree]++;
            BFS_Count++;
        }
    }
    while (BFS_Count_List[BFS_Degree]) {
        BFS_Count_List[BFS_Degree + 1] = 0;
        for (k = 0; k < BFS_Count_List[BFS_Degree]; k++) {
            xx = BFS_List[BFS_Degree][k].x;
            yy = BFS_List[BFS_Degree][k].y;
            for (kk = 0; kk < 4; kk++) {
                if (Check_Equal[kk](xx, yy)) {
                    Ptr_BFS = &BFS_List[BFS_Degree + 1][BFS_Count_List[BFS_Degree + 1]];
                    Ptr_BFS->x = xx + Def_Drt[kk].x;
                    Ptr_BFS->y = yy + Def_Drt[kk].y;
                    Ptr_BFS->Drt = Reverse_Drt[kk];
                    BFS_Count_List[BFS_Degree + 1]++;
                    BFS_Count++;
                }
            }
        }
        BFS_Degree++;
    }
    for (k = 0; k < BFS_Count_List_Mark; k++)
        BFS_Mark[BFS_List_Mark[k].y][BFS_List_Mark[k].x] = 0;
    if (BFS_Count >= 2)
        return 1;
    return 0;
}

int BFS_Find(char x, char y) {
    BFS_X = x;
    BFS_Y = y;
    Current_Value = Map[y][x];
    BFS_Degree = 0;
    BFS_Count = 0;
    BFS_Count_List[BFS_Degree] = 0;
    BFS_List_Mark[0].x = x;
    BFS_List_Mark[0].y = y;
    BFS_Count_List_Mark = 1;
    BFS_Mark[y][x] = 1;
    for (kk = 0; kk < 4; kk++) {
        if (Check_Equal[kk](x, y)) {
            Ptr_BFS = &BFS_List[BFS_Degree][BFS_Count_List[BFS_Degree]];
            Ptr_BFS->x = x + Def_Drt[kk].x;
            Ptr_BFS->y = y + Def_Drt[kk].y;
            Ptr_BFS->Drt = Reverse_Drt[kk];
            BFS_Count_List[BFS_Degree]++;
            BFS_Count++;
        }
    }
    while (BFS_Count_List[BFS_Degree]) {
        BFS_Count_List[BFS_Degree + 1] = 0;
        for (k = 0; k < BFS_Count_List[BFS_Degree]; k++) {
            xx = BFS_List[BFS_Degree][k].x;
            yy = BFS_List[BFS_Degree][k].y;
            for (kk = 0; kk < 4; kk++) {
                if (Check_Equal[kk](xx, yy)) {
                    Ptr_BFS = &BFS_List[BFS_Degree + 1][BFS_Count_List[BFS_Degree + 1]];
                    Ptr_BFS->x = xx + Def_Drt[kk].x;
                    Ptr_BFS->y = yy + Def_Drt[kk].y;
                    Ptr_BFS->Drt = Reverse_Drt[kk];
                    BFS_Count_List[BFS_Degree + 1]++;
                    BFS_Count++;
                }
            }
        }
        BFS_Degree++;
    }
    for (k = 0; k < BFS_Count_List_Mark; k++)
        BFS_Mark[BFS_List_Mark[k].y][BFS_List_Mark[k].x] = 0;
    if (BFS_Count >= 2) {
        Combo++;
        if (Combo == MAX_COMBO)
            Combo = MAX_COMBO - 1;
        Play_Sound(Combo);
        Increase_Score(Combo * BFS_Count);
        if (Combo > 1 && Life < MAX_LIFE)
            Life += Increase_Life;
        Count_Joint_Square = 0;
        for (k = 0; k < BFS_Degree - 1; k++)
            for (kk = 0; kk < BFS_Count_List[k]; kk++)
                Create_Joint_Square[BFS_List[k][kk].Drt](&BFS_List[k][kk].x, &BFS_List[k][kk].y);
        BFS_Degree_Save = BFS_Degree;
        BFS_Degree--;
        Pos_Effect = 0;
        Count_Animation = 0;
        for (i = 0; i < BFS_Count_List[BFS_Degree]; i++)
            Create_Animation(BFS_List[BFS_Degree][i].x, BFS_List[BFS_Degree][i].y, BFS_List[BFS_Degree][i].Drt);
        X_Current = x;
        Y_Current = y;
        return 1;
    }
    return 0;
}

void Make_Block_Fall() {
    BFS_Degree = BFS_Degree_Save;
    for (k = 0; k < MAX; k++)
        Mark_Column_Fall[k] = -1;
    if (Mark_Column_Fall[BFS_X] < BFS_Y) {
        Mark_Column_Fall[BFS_X] = BFS_Y;
        if (Mark_Column_Fall_Full[BFS_X] < BFS_Y)
            Mark_Column_Fall_Full[BFS_X] = BFS_Y;
    }
    for (k = 0; k < BFS_Degree; k++)
        for (kk = 0; kk < BFS_Count_List[k]; kk++) {
            if (Mark_Column_Fall[BFS_List[k][kk].x] < BFS_List[k][kk].y) {
                Mark_Column_Fall[BFS_List[k][kk].x] = BFS_List[k][kk].y;
                if (Mark_Column_Fall_Full[BFS_List[k][kk].x] < BFS_List[k][kk].y)
                    Mark_Column_Fall_Full[BFS_List[k][kk].x] = BFS_List[k][kk].y;
            }
        }
    Pos_Block_Fall = Count_Done_Block_Fall = Count_Block_Make = 0;
    for (k = 0; k < MAX; k++) {
        if (Mark_Column_Fall[k] != -1) {
            kk = Mark_Column_Fall[k];
            Column_Need = 0;
            while (kk >= 0) {
                if (Map[kk][k] == -1) {
                    Column_Need++;
                    ll = kk - 1;
                    while (ll >= 0) {
                        if (Map[ll][k] > -1) {
                            Create_Block_Fall(k, ll, Map[ll][k], kk - ll);
                            Map[ll][k] = -1;
                            Column_Need--;
                            break;
                        }
                        ll--;
                    }
                }
                kk--;
            }
            for (kk = 0; kk < Column_Need; kk++)
                Create_Block_Fall(k, -1 - kk, rand() % MAX_RANDOM, Column_Need);
        }
    }
}

void Make_Block_Zoom() {
    BFS_Degree = BFS_Degree_Save;
    Status_Zoom = 0;
    Count_Block_Make = 0;
    if (Mark_Block_Zoom_Full[BFS_Y][BFS_X] == 0) {
        Mark_Block_Zoom_Full[BFS_Y][BFS_X] = 1;
        List_Block_Zoom_Full[Count_Block_Zoom_Full].x = BFS_X;
        List_Block_Zoom_Full[Count_Block_Zoom_Full].y = BFS_Y;
        Count_Block_Zoom_Full++;
    }
    for (k = 0; k < BFS_Degree; k++)
        for (kk = 0; kk < BFS_Count_List[k]; kk++) {
            Ptr_BFS = &BFS_List[k][kk];
            Block_Zoom[Count_Block_Make].Color = rand() % MAX_RANDOM;
            Block_Zoom[Count_Block_Make].x = Ptr_BFS->x;
            Block_Zoom[Count_Block_Make].y = Ptr_BFS->y;
            Count_Block_Make++;
            if (Mark_Block_Zoom_Full[Ptr_BFS->y][Ptr_BFS->x] == 0) {
                Mark_Block_Zoom_Full[Ptr_BFS->y][Ptr_BFS->x] = 1;
                List_Block_Zoom_Full[Count_Block_Zoom_Full].x = Ptr_BFS->x;
                List_Block_Zoom_Full[Count_Block_Zoom_Full].y = Ptr_BFS->y;
                Count_Block_Zoom_Full++;
            }
        }
}

void Init_Block_Num() {
    int x, y;
    int Size_Img = SIZE_CELL * SIZE_CELL * 4;
    y = (SIZE_CELL - Img_Num[0].h) / 2;
    for (i = 0; i < MAX_NUM; i++) {
        Set_Image(&Img_Block_Num[i], &Img_Block_Full);
        j = (i + 1) / 10;
        k = (i + 1) % 10;
        if (j > 0) {
            x = (SIZE_CELL - Img_Num[k].w - Img_Num[j].w) / 2;
            Mix_Image_Color(&Img_Block_Num[i], &Img_Num[j], x, y, NUM_COLOR);
            x += Img_Num[j].w;
            Mix_Image_Color(&Img_Block_Num[i], &Img_Num[k], x, y, NUM_COLOR);
        } else {
            x = (SIZE_CELL - Img_Num[k].w) / 2;
            Mix_Image_Color(&Img_Block_Num[i], &Img_Num[k], x, y, NUM_COLOR);
        }
    }
    free(Img_Block_Full.img);
}

void Init_Game() {
    Load_Texture(&Img_Logo, "Images/Menu/Logo.png");
    Load_Texture(&Img_Select_No, "Images/Menu/Select_No.png");
    Load_Texture(&Img_Select_Yes, "Images/Menu/Select_Yes.png");
    Load_Texture(&Img_Fall_Down, "Images/Menu/Fall_Down.png");
    Load_Texture(&Img_Zoom_In, "Images/Menu/Zoom_In.png");
    Load_Texture(&Img_Easy, "Images/Menu/Easy.png");
    Load_Texture(&Img_Hard, "Images/Menu/Hard.png");
    Load_Texture(&Img_Title_Page[0], "Images/Menu/Mode.png");
    Load_Texture(&Img_Title_Page[1], "Images/Menu/Level.png");
    Load_Texture(&Img_Block_Full, "Images/Game/Block_Full.png");
    Load_Texture(&Img_Block_Ripple[0], "Images/Game/Block_Ripple_Up.png");
    Load_Texture(&Img_Block_Tail[0], "Images/Game/Block_Tail_Up.png");
    Load_Texture(&Img_Block_Ripple[1], "Images/Game/Block_Ripple_Down.png");
    Load_Texture(&Img_Block_Tail[1], "Images/Game/Block_Tail_Down.png");
    Load_Texture(&Img_Block_Ripple[2], "Images/Game/Block_Ripple_Left.png");
    Load_Texture(&Img_Block_Tail[2], "Images/Game/Block_Tail_Left.png");
    Load_Texture(&Img_Block_Ripple[3], "Images/Game/Block_Ripple_Right.png");
    Load_Texture(&Img_Block_Tail[3], "Images/Game/Block_Tail_Right.png");
    Load_Texture(&Img_Life, "Images/Game/Life.png");
    Load_Texture(&Img_Num[0], "Images/Game/Num_0.png");
    Load_Texture(&Img_Num[1], "Images/Game/Num_1.png");
    Load_Texture(&Img_Num[2], "Images/Game/Num_2.png");
    Load_Texture(&Img_Num[3], "Images/Game/Num_3.png");
    Load_Texture(&Img_Num[4], "Images/Game/Num_4.png");
    Load_Texture(&Img_Num[5], "Images/Game/Num_5.png");
    Load_Texture(&Img_Num[6], "Images/Game/Num_6.png");
    Load_Texture(&Img_Num[7], "Images/Game/Num_7.png");
    Load_Texture(&Img_Num[8], "Images/Game/Num_8.png");
    Load_Texture(&Img_Num[9], "Images/Game/Num_9.png");
    Load_Texture(&Img_Home, "Images/Game/Home.png");
    Load_Texture(&Img_Game_Over, "Images/Game/Game_Over.png");
    Load_Sound();
    Init_Block_Num();
    Init_Menu_Image();
    Init_Game_Image();
    Init_Zoom_Position();
    Init_Pos_LRBT();
    Init_Pos_Offset();
    Main_Menu();
    Init_Pos_Ani[0] = &Init_Pos_Ani_Up;
    Init_Pos_Ani[1] = &Init_Pos_Ani_Down;
    Init_Pos_Ani[2] = &Init_Pos_Ani_Left;
    Init_Pos_Ani[3] = &Init_Pos_Ani_Right;
    Action_Effect_Ani[0] = &Action_Effect_Ani_Up;
    Action_Effect_Ani[1] = &Action_Effect_Ani_Down;
    Action_Effect_Ani[2] = &Action_Effect_Ani_Left;
    Action_Effect_Ani[3] = &Action_Effect_Ani_Right;
    Check_Equal[0] = &Check_Equal_Up;
    Check_Equal[1] = &Check_Equal_Down;
    Check_Equal[2] = &Check_Equal_Left;
    Check_Equal[3] = &Check_Equal_Right;
    Create_Joint_Square[0] = &Create_Joint_Square_Up;
    Create_Joint_Square[1] = &Create_Joint_Square_Down;
    Create_Joint_Square[2] = &Create_Joint_Square_Left;
    Create_Joint_Square[3] = &Create_Joint_Square_Right;
    Display_Block_Make[0] = &Draw_Block_Fall;
    Display_Block_Make[1] = &Draw_Block_Zoom;
    Make_Block_Appear[0] = &Make_Block_Fall;
    Make_Block_Appear[1] = &Make_Block_Zoom;
    Timer_Block_Make[0] = &Timer_Block_Fall;
    Timer_Block_Make[1] = &Timer_Block_Zoom;
    for (i = 0; i < MAX; i++)
        for (j = 0; j < MAX; j++) {
            BFS_Mark[i][j] = 0;
            Mark_Block_Zoom_Full[i][j] = 0;
        }
    for (i = 0; i < MAX_NUM; i++)
        Next_Number[i] = i + 1;
    Next_Number[MAX_NUM - 1] = 0;
}

void Init_GL() {
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, WIDTH, HEIGHT, 0);
    glViewport(0, 0, WIDTH, HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glClearColor(0.2f, 0.6f, 0.6f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    Init_Game();
}

void Display_Menu() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    Map_Texture(&Img_Logo);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(Rect_Logo.Left, Rect_Logo.Bottom);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(Rect_Logo.Right, Rect_Logo.Bottom);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(Rect_Logo.Right, Rect_Logo.Top);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(Rect_Logo.Left, Rect_Logo.Top);
    glEnd();
    Map_Texture(&Img_Title_Page[Menu_Page]);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(Title_Page_Left, Title_Page_Bottom);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(Title_Page_Right[Menu_Page], Title_Page_Bottom);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(Title_Page_Right[Menu_Page], Title_Page_Top);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(Title_Page_Left, Title_Page_Top);
    glEnd();
    for (i = 0; i < 2; i++) {
        Tmp_Int = Select_Choice[i];
        Map_Texture(&Img_Select_Menu[Menu_Page][i][Tmp_Int]);
        glBegin(GL_POLYGON);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(Select_Left + Select_Offset_Left[Tmp_Int], Select_Bottom[i] + Select_Offset_Bottom[Tmp_Int]);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(Select_Right + Select_Offset_Right[Tmp_Int], Select_Bottom[i] + Select_Offset_Bottom[Tmp_Int]);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(Select_Right + Select_Offset_Right[Tmp_Int], Select_Top[i] + Select_Offset_Top[Tmp_Int]);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(Select_Left + Select_Offset_Left[Tmp_Int], Select_Top[i] + Select_Offset_Top[Tmp_Int]);
        glEnd();
    }
    glutSwapBuffers();
}

void Display_Game() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    Display_Block_Make[Game_Mode]();
    if (!Check_Done_Effect()) {
        for (i = 0; i < Count_Animation; i++) {
            glColor3fv(Color_Block[Animation[i].Color]);
            Draw_Effect_Ani(&Animation[i]);
        }
    }
    if (Count_Joint_Square > 0) {
        glDisable(GL_TEXTURE_2D);
        for (i = 0; i < Count_Joint_Square; i++) {
            glBegin(GL_POLYGON);
            glVertex2f(Joint_Square[i].Left, Joint_Square[i].Bottom);
            glVertex2f(Joint_Square[i].Right, Joint_Square[i].Bottom);
            glVertex2f(Joint_Square[i].Right, Joint_Square[i].Top);
            glVertex2f(Joint_Square[i].Left, Joint_Square[i].Top);
            glEnd();
        }
        glEnable(GL_TEXTURE_2D);
    }
    for (i = 0; i < MAX; i++) {
        for (j = 0; j < MAX; j++) {
            if (Map[i][j] != -1) {
                Map_Texture(&Img_Block_Num[Map[i][j]]);
                glColor3fv(Color_Block[Map[i][j]]);
                glBegin(GL_POLYGON);
                glTexCoord2f(0.0f, 0.0f);
                glVertex2f(Pos_Left[j], Pos_Bottom[i]);
                glTexCoord2f(1.0f, 0.0f);
                glVertex2f(Pos_Right[j], Pos_Bottom[i]);
                glTexCoord2f(1.0f, 1.0f);
                glVertex2f(Pos_Right[j], Pos_Top[i]);
                glTexCoord2f(0.0f, 1.0f);
                glVertex2f(Pos_Left[j], Pos_Top[i]);
                glEnd();
            }
        }
    }
    glColor3fv(Color_Block[4]);
    Map_Texture(&Img_Life);
    for (i = 0; i < Life; i++) {
        glBegin(GL_POLYGON);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(Life_Left[i], Life_Bottom);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(Life_Right[i], Life_Bottom);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(Life_Right[i], Life_Top);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(Life_Left[i], Life_Top);
        glEnd();
    }
    glColor3fv(Color_Block[1]);
    for (i = 0; i < 5; i++) {
        Score_Right[i + 1] = Score_Right[i] - Img_Num[Score[i]].w;
        Map_Texture(&Img_Num[Score[i]]);
        glBegin(GL_POLYGON);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(Score_Right[i + 1], Score_Bottom);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(Score_Right[i], Score_Bottom);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(Score_Right[i], Score_Top);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(Score_Right[i + 1], Score_Top);
        glEnd();
    }
    glColor3fv(Color_Block[3]);
    Map_Texture(&Img_Home);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(Rect_Home.Left, Rect_Home.Bottom);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(Rect_Home.Right, Rect_Home.Bottom);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(Rect_Home.Right, Rect_Home.Top);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(Rect_Home.Left, Rect_Home.Top);
    glEnd();
    if (Game_Over) {
        glColor3f(1.0f, 1.0f, 1.0f);
        Ptr_Rect = &Rect_Game_Over[Stt_Game_Over];
        Map_Texture(&Img_Game_Over);
        glBegin(GL_POLYGON);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(Ptr_Rect->Left, Ptr_Rect->Bottom);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(Ptr_Rect->Right, Ptr_Rect->Bottom);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(Ptr_Rect->Right, Ptr_Rect->Top);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(Ptr_Rect->Left, Ptr_Rect->Top);
        glEnd();
    }
    glutSwapBuffers();
}

void Resize(int x, int y) {
    glutPositionWindow(POS_X, POS_Y);
    glViewport(0, y - HEIGHT, WIDTH, HEIGHT);
    //glutReshapeWindow(WIDTH, HEIGHT);
}

void Main_Menu() {
    Func_Timer = &Func_Timer_Null;
    Menu_Page = 0;
    Menu_Choice = 0;
    Select_Choice[0] = 1;
    Select_Choice[1] = 0;
    glutDisplayFunc(Display_Menu);
    glutMouseFunc(Mouse_Menu);
    glutPassiveMotionFunc(Passive_Menu);
    glColor3f(1.0f, 1.0f, 1.0f);
    glutPostRedisplay();
}

void Init_Map() {
    for (i = 0; i < MAX; i++)
        for (j = 0; j < MAX; j++)
            Map[i][j] = -1;
    for (i = 0; i < MAX; i++)
        for (j = 0; j < MAX; j++) {
            do {
                Map[i][j] = rand() % MAX_RANDOM;
            } while (BFS_Init(j, i));
        }
}

void In_Game() {
    Game_Over = 0;
    Stt_Game_Over = 0;
    glutPassiveMotionFunc(NULL);
    glutMouseFunc(Mouse_Game);
    glutDisplayFunc(Display_Game);
    Func_Timer = &Timer;
    for (i = 0; i < 5; i++)
        Score[i] = 0;
    Life = MAX_LIFE;
    Init_Map();
    glutTimerFunc(0, Func_Timer, 0);
}

void Make_Game_Over() {
    Play_Sound_Game_Over();
    glutMouseFunc(NULL);
    Game_Over = 1;
    Stt_Game_Over = 0;
    Func_Timer = &Timer_Game_Over;
}

void Passive_Menu(int x, int y) {
    if (Select_Region_Left <= x && x <= Select_Region_Right) {
        for (i = 0; i < 2; i++) {
            if (Select_Region_Bottom[i] <= y && y <= Select_Region_Top[i]) {
                if (i != Menu_Choice) {
                    Select_Choice[Menu_Choice] = 0;
                    Select_Choice[i] = 1;
                    Menu_Choice = i;
                    glutPostRedisplay();
                }
                break;
            }
        }
    }
}

void Mouse_Menu(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (Select_Region_Left <= x && x <= Select_Region_Right) {
            for (i = 0; i < 2; i++) {
                if (Select_Region_Bottom[i] <= y && y <= Select_Region_Top[i]) {
                    Menu_Page++;
                    switch (Menu_Page) {
                    case 1:
                        Game_Mode = i;
                        break;
                    case 2:
                        Increase_Life = 1 - i;
                        In_Game();
                        break;
                    }
                    glutPostRedisplay();
                    break;
                }
            }
        }
    }
}

void Mouse_Game(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (x >= START_X && x <= FULL_X && y >= START_Y && y <= FULL_Y) {
            Tmp_Int = (x - START_X) / SIZE_CELL_FULL;
            Tmp_Int_2 = (y - START_Y) / SIZE_CELL_FULL;
            Map[Tmp_Int_2][Tmp_Int] = Next_Number[Map[Tmp_Int_2][Tmp_Int]];
            glutPostRedisplay();
            Play_Sound(0);
            Combo = 0;
            if (BFS_Find(Tmp_Int, Tmp_Int_2)) {
                for (ii = 0; ii < MAX; ii++)
                    Mark_Column_Fall_Full[ii] = -1;
                glutMouseFunc(NULL);
            } else {
                Life--;
                if (Life == 0)
                    Make_Game_Over();
            }
        } else if (Reg_Home.Left <= x && x <= Reg_Home.Right && Reg_Home.Bottom <= y && y <= Reg_Home.Top) {
            Main_Menu();
        }
    }
}

int Timer_Block_Fall() {
    if (Count_Done_Block_Fall < Count_Block_Make) {
        Pos_Block_Fall++;
        for (i = 0; i < Count_Block_Make; i++) {
            if (Block_Fall[i].IsAlive) {
                Block_Fall[i].Bottom += OFFSET_FALL;
                Block_Fall[i].Top += OFFSET_FALL;
                if (Pos_Block_Fall == Block_Fall[i].Count) {
                    Map[Block_Fall[i].y_New][Block_Fall[i].x] = Block_Fall[i].Value;
                    Block_Fall[i].IsAlive = 0;
                    Count_Done_Block_Fall++;
                    if (Count_Done_Block_Fall == Count_Block_Make) {
                        Count_Block_Make = 0;
                        Tmp_Int = 0;
                        BFS_Result = 0;
                        for (mm = 0; mm < MAX; mm++) {
                            for (nn = Mark_Column_Fall_Full[BFS_Pos_Find[mm]]; nn >= 0; nn--) {
                                BFS_Result = BFS_Find(BFS_Pos_Find[mm], nn);
                                if (BFS_Result)
                                    break;
                            }
                            if (BFS_Result)
                                break;
                        }
                        if (!BFS_Result)
                            glutMouseFunc(Mouse_Game);
                    }
                }
            }
        }
        return 1;
    }
    return 0;
}

int Timer_Block_Zoom() {
    int i;
    if (Status_Zoom < MAX_ZOOM) {
        Status_Zoom++;
        if (Status_Zoom == MAX_ZOOM) {
            for (i = 0; i < Count_Block_Make; i++)
                Map[Block_Zoom[i].y][Block_Zoom[i].x] = Block_Zoom[i].Color;
            BFS_Result = 0;
            for (i = 0; i < Count_Block_Zoom_Full; i++) {
                BFS_Result = BFS_Find(List_Block_Zoom_Full[i].x, List_Block_Zoom_Full[i].y);
                if (BFS_Result)
                    break;
            }
            if (!BFS_Result) {
                for (i = 0; i < Count_Block_Zoom_Full; i++)
                    Mark_Block_Zoom_Full[List_Block_Zoom_Full[i].y][List_Block_Zoom_Full[i].x] = 0;
                Count_Block_Zoom_Full = 0;
                glutMouseFunc(Mouse_Game);
            }
            Count_Block_Make = 0;
        }
        return 1;
    }
    return 0;
}

void Timer_Game_Over(int value) {
    if (Stt_Game_Over < MAX_STT_GAME_OVER) {
        Stt_Game_Over++;
        if (Stt_Game_Over == MAX_STT_GAME_OVER)
            Stt_Wait_Game_Over = 0;
    } else {
        if (Stt_Wait_Game_Over < WAIT_GAME_OVER) {
            Stt_Wait_Game_Over++;
            if (Stt_Wait_Game_Over == WAIT_GAME_OVER)
                Main_Menu();
        }
    }
    glutPostRedisplay();
    glutTimerFunc(INTERVAL, Func_Timer, 0);
}

void Timer(int value) {
    IsRedisplay = 0;
    if (!Check_Done_Effect()) {
        Pos_Effect++;
        for (i = 0; i < Count_Animation; i++) {
            Action_Effect_Ani[Animation[i].Drt](&Animation[i]);
        }
        if (Check_Done_Effect()) {
            BFS_Degree--;
            Count_Joint_Square -= BFS_Count_List[BFS_Degree];
            if (BFS_Degree > -1) {
                Pos_Effect = 0;
                Count_Animation = 0;
                for (i = 0; i < BFS_Count_List[BFS_Degree]; i++)
                    Create_Animation(BFS_List[BFS_Degree][i].x, BFS_List[BFS_Degree][i].y, BFS_List[BFS_Degree][i].Drt);
            } else {
                Map[Y_Current][X_Current] = Next_Number[Map[Y_Current][X_Current]];
                Make_Block_Appear[Game_Mode]();
            }
        }
        IsRedisplay = 1;
    } else if (Timer_Block_Make[Game_Mode]())
        IsRedisplay = 1;
    if (IsRedisplay)
        glutPostRedisplay();
    glutTimerFunc(INTERVAL, Func_Timer, 0);
}

int main(int argc, char **argv) {
    srand(time(NULL));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowPosition(POS_X, POS_Y);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Merge Number");
    Init_GL();
    glutDisplayFunc(Display_Menu);
    glutPassiveMotionFunc(Passive_Menu);
    glutMouseFunc(Mouse_Menu);
    glutReshapeFunc(Resize);
    glutMainLoop();
    return 0;
}
