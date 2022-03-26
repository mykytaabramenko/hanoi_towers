#include <iostream>
#include <windows.h>
#include <conio.h>
#include <cmath>
#include <vector>

#define MENUATTR 241
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define ENTER 13
#define ESC 27
#define DEF 240

using namespace std;

struct Rect{
   int length;
   WORD color;
   bool select;
   bool bottom;
   COORD coord;
};

char *menu[] = {"Play", "How to play?", "Watch the correct algorithm", "Exit"};

CONSOLE_SCREEN_BUFFER_INFO cs;

HANDLE h;

vector <Rect> pyramids[3];

COORD center, pyramid_null_coord, hinges[3];

int length, maxheight, hinge_top_Y, steps = 0;

WORD color = 1;

void menu_function();

void draw_exit(int);

void drawbox();

void draw_hinge();

void draw_rectangle(Rect);

void draw_numbers(int, int);

void draw_submenu();

void draw_table();

void destroy_rectangle(Rect);

void play(int);

void submenu();

void how_to_play();

void draw_exit(int);

void drawmenu(int);

inline int min(int a, int b){
    return a<b?a:b;
}

void move(int, int);

void hanoi(int, int, int);

void correct_algorithm(int);

void watch_the_coorect_algoritm();

int main(){
    system("title Hanoi tower");
    h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, DEF);
    system("cls");
    GetConsoleScreenBufferInfo(h, &cs);

    center = {cs.srWindow.Right/2, cs.srWindow.Bottom/2};
    length = (0.8*(cs.srWindow.Right - 10))/3;
    if(length%2 != 0) length++;
    pyramid_null_coord  = {0.1*cs.srWindow.Right + 0.8, 0.8*cs.srWindow.Bottom};
    hinges[0] = {pyramid_null_coord.X + length/2 - 1, pyramid_null_coord.Y};
    hinges[1] = {hinges[0].X + length + 2, pyramid_null_coord.Y};
    hinges[2] = {hinges[1].X + length + 2, pyramid_null_coord.Y};

    maxheight = min((int) (length + 4)/4 - 1, (int) (pyramid_null_coord.Y - 6)/2);
    hinge_top_Y = pyramid_null_coord.Y - 2*maxheight - 1;

    menu_function();

    return 0;

}

void drawbox(){
    DWORD lp;
    char c = 201;
    cout << c;
    c = 205;
    FillConsoleOutputCharacter(h, c, cs.srWindow.Right - 1, {1, 0}, &lp);
    FillConsoleOutputCharacter(h, c, cs.srWindow.Right - 1, {1, cs.srWindow.Bottom}, &lp);
    
    c = 187;
    SetConsoleCursorPosition(h, {cs.srWindow.Right});
    cout << c;

    c = 186;
    COORD temp = {0, 1};
    for(int i = 0; i < cs.srWindow.Bottom - 1; i++){
        SetConsoleCursorPosition(h, temp);
        cout << c;
        temp.X = cs.srWindow.Right;
        SetConsoleCursorPosition(h, temp);
        cout << c;
        temp.Y++;
        temp.X = 0;
    }
    
    c = 200;
    cout << c;
    SetConsoleCursorPosition(h, {cs.srWindow.Right, cs.srWindow.Bottom});
    c = 188;
    cout << c;
}

void menu_function() {
    drawbox();
    char button;
    int pointer = 0;
    int exit_ptr = 0;
    
    COORD temp;
    do{
        drawmenu(pointer);
        button = getch();
        switch (button){
            case DOWN:
            pointer++;
            if(pointer == 4) pointer = 0;      
            drawmenu(pointer);
            break;
            case UP:
            pointer--;
            if(pointer == -1) pointer = 3;
            drawmenu(pointer);
            break;
            case ENTER:
            system("cls");
            drawbox();
            switch(pointer){
                case 0: 
                submenu();
                pointer = 0;
                break;
                case 1: 
                how_to_play();
                system("cls");
                drawbox();
                pointer = 0;
                break;
                case 2: 
                watch_the_coorect_algoritm();
                pointer = 0;
                break;
                case 3: 
                system("cls");
                drawbox();
                draw_exit(exit_ptr);
                do{
                    button = getch();
                    switch(button){
                        case LEFT:
                        exit_ptr--;
                        if(exit_ptr == -1) exit_ptr = 1;
                        draw_exit(exit_ptr);
                        break;
                        case RIGHT:
                        exit_ptr++;
                        if(exit_ptr == 2) exit_ptr = 0;
                        draw_exit(exit_ptr);
                        break;
                        case ENTER:
                        switch(exit_ptr){
                            case 0: 
                            exit(0);
                            break;
                            case 1: 
                            system("cls");
                            exit_ptr = -1;
                            pointer = 0;
                            break;
                        }
                    }   
                }while(exit_ptr != -1);
                exit_ptr = 0;
                drawbox();
                break;
            }
            break;
            case ESC:
            system("cls");
            drawbox();
            draw_exit(exit_ptr);
            do{
                button = getch();
                switch(button){
                    case LEFT:
                    exit_ptr--;
                    if(exit_ptr == -1) exit_ptr = 1;
                    draw_exit(exit_ptr);
                    break;
                    case RIGHT:
                    exit_ptr++;
                    if(exit_ptr == 2) exit_ptr = 0;
                    draw_exit(exit_ptr);
                    break;
                    case ENTER:
                    switch(exit_ptr){
                        case 0: 
                        exit(0);
                        break;
                        case 1: 
                        system("cls");
                        exit_ptr = -1;
                        pointer = 0;
                        break;
                    }
                }   
            }while(exit_ptr != -1);
            exit_ptr = 0;
            drawbox();
            break;  
        }
    }while(1);
}

void drawmenu(int draw){
    COORD remember;
    COORD temp;

    for(int i = 0, j = -2, k = 0; i < 4; i++, j++, k++){
        temp = {center.X - strlen(menu[i])/2, center.Y + j};
        SetConsoleCursorPosition(h, temp);
        if(k == draw){
            SetConsoleTextAttribute(h, MENUATTR);
            cout << menu[i] << " ";
            remember.X = temp.X + strlen(menu[i]);
            remember.Y = temp.Y;
            SetConsoleTextAttribute(h, DEF);
        }else cout << menu[i] << " ";

    }

    SetConsoleCursorPosition(h, remember);
}

void draw_exit(int exit_ptr){
    COORD temp, remember;
    temp = center;
    temp.X -= strlen("Are you sure you want to exit?")/2;
    SetConsoleCursorPosition(h, temp);
    cout << "Are you sure you want to exit?";
    if(exit_ptr == 0){
        temp.Y++;
        temp.X = center.X - 10;
        SetConsoleCursorPosition(h, temp);
        SetConsoleTextAttribute(h, MENUATTR);
        cout << "Yes";
        temp.X += 3;
        remember = temp;
        temp.X = center.X + 10;
        SetConsoleCursorPosition(h, temp);
        SetConsoleTextAttribute(h, DEF);
        cout << "No";
    }
    if(exit_ptr == 1){
        temp.Y++;
        temp.X = center.X - 10;
        SetConsoleCursorPosition(h, temp);
        SetConsoleTextAttribute(h, DEF);
        cout << "Yes";
        temp.X = center.X + 10;
        SetConsoleCursorPosition(h, temp);
        SetConsoleTextAttribute(h, MENUATTR);
        cout << "No";
        temp.X += 2;
        remember = temp;
        SetConsoleTextAttribute(h, DEF);
    }
    SetConsoleCursorPosition(h, remember);
}

void draw_rectangle(Rect rec){
    DWORD lp;
    if(rec.select){
        DWORD lp;
        SetConsoleTextAttribute(h, rec.color);
        SetConsoleCursorPosition(h, rec.coord);
        char c =  200;
        cout << c;
        SetConsoleCursorPosition(h, {rec.coord.X, rec.coord.Y - 1});
        c = 201;
        cout << c;
        c = 205;
        FillConsoleOutputCharacter(h, c, rec.length - 2, {rec.coord.X + 1, rec.coord.Y}, &lp);
        FillConsoleOutputAttribute(h, rec.color, rec.length - 2, {rec.coord.X + 1, rec.coord.Y}, &lp);
        FillConsoleOutputCharacter(h, c, rec.length - 2, {rec.coord.X + 1, rec.coord.Y - 1}, &lp);
        FillConsoleOutputAttribute(h, rec.color, rec.length - 2, {rec.coord.X + 1, rec.coord.Y - 1}, &lp);
        c = 188;
        SetConsoleCursorPosition(h, {rec.coord.X + rec.length - 1, rec.coord.Y});
        cout << c;
        SetConsoleCursorPosition(h, {rec.coord.X + rec.length - 1, rec.coord.Y - 1});
        c = 187;
        cout << c;
        SetConsoleTextAttribute(h, DEF);
    }else{
        FillConsoleOutputCharacter(h, ' ', rec.length, rec.coord, &lp);
        FillConsoleOutputCharacter(h, ' ', rec.length, {rec.coord.X, rec.coord.Y - 1}, &lp);
        FillConsoleOutputAttribute(h, rec.color, rec.length, rec.coord, &lp);
        if(lp != rec.length){
            system("cls");
            SetConsoleCursorPosition(h, {center.X - strlen("Sorry, some mistake"), center.Y});
            cout << "Sorry, some mistake" << endl;
            Sleep(100);
            exit(0);
        }
        FillConsoleOutputAttribute(h, rec.color, rec.length, {rec.coord.X, rec.coord.Y - 1}, &lp);
        if(lp != rec.length){
            system("cls");
            SetConsoleCursorPosition(h, {center.X - strlen("Sorry, some mistake"), center.Y});
            cout << "Sorry, some mistake" << endl;
            Sleep(100);
            exit(0);
        }
    }
}

void draw_numbers(int maxheight, int number){
    COORD temp_coord = {center.X - (maxheight - 1), center.Y + 1};
    COORD remember;
    remember.Y = temp_coord.Y;
    SetConsoleCursorPosition(h, temp_coord);

    for(int i = 0; i < maxheight; i++){
        if(i == number){
            SetConsoleTextAttribute(h, MENUATTR);
            cout << i + 1;
            SetConsoleTextAttribute(h, DEF);
            remember.X = temp_coord.X + 2*i;
        }else cout << i + 1;
        cout << ' ';
    }

    SetConsoleCursorPosition(h, remember);
}

void draw_submenu(){
    COORD temp_coord = {center.X - strlen("What height do you want?")/2, center.Y};
    SetConsoleCursorPosition(h, temp_coord);
    cout << "What height do you want?" << endl;
}

void draw_table(){
    COORD temp = {pyramid_null_coord.X - 2, pyramid_null_coord.Y + 1};
    DWORD lp;
    
    FillConsoleOutputAttribute(h, 96, 4 + 4 + 3*length, {pyramid_null_coord.X - 2, pyramid_null_coord.Y + 1}, &lp);
    if(lp != 4 + 4 + 3*length){
        system("cls");
            drawbox();
            SetConsoleCursorPosition(h, {center.X - strlen("Sorry, some mistake")/2, center.Y});
            cout << "Sorry, some mistake";
            Sleep(1500);
            exit(0);  
    }
    FillConsoleOutputAttribute(h, 96, 8 + 3*length, {pyramid_null_coord.X - 2, pyramid_null_coord.Y + 2}, &lp);
    if(lp != 8 + 3*length){
        system("cls");
            drawbox();
            SetConsoleCursorPosition(h, {center.X - strlen("Sorry, some mistake")/2, center.Y});
            cout << "Sorry, some mistake";
            Sleep(1500);
            exit(0);  
    }
    for(int i = pyramid_null_coord.Y + 3; i < cs.srWindow.Bottom; i++){
        FillConsoleOutputAttribute(h, 96, length/3, {hinges[0].X + 1 ,i}, &lp);
        if(lp != length/3){
            system("cls");
            drawbox();
            SetConsoleCursorPosition(h, {center.X - strlen("Sorry, some mistake")/2, center.Y});
            cout << "Sorry, some mistake";
            Sleep(1500);
            exit(0);  
        }   
    }

    for(int i = pyramid_null_coord.Y + 3; i < cs.srWindow.Bottom; i++){
        FillConsoleOutputAttribute(h, 96, length/3, {hinges[2].X - 1 - length/3, i}, &lp);
        if(lp != length/3){
            system("cls");
            drawbox();
            SetConsoleCursorPosition(h, {center.X - strlen("Sorry, some mistake")/2, center.Y});
            cout << "Sorry, some mistake";
            Sleep(1500);
            exit(0);  
        }   
    }

}

void destroy_rectangle(Rect rec){
    DWORD lp;
    char c = ' ';

    FillConsoleOutputCharacter(h, c, rec.length, rec.coord, &lp);
    FillConsoleOutputCharacter(h, c, rec.length, {rec.coord.X, rec.coord.Y - 1}, &lp);
    FillConsoleOutputAttribute(h, DEF, rec.length, rec.coord, &lp);
    FillConsoleOutputAttribute(h, DEF, rec.length, {rec.coord.X, rec.coord.Y - 1}, &lp);
    
    if(rec.coord.Y >= hinge_top_Y && rec.coord.Y <= pyramid_null_coord.Y){
        c = 195;
        SetConsoleCursorPosition(h, {rec.coord.X + rec.length/2 - 1, rec.coord.Y});
        cout << c;
        c = 180;
        cout << c;

        c = 195;
        SetConsoleCursorPosition(h, {rec.coord.X + rec.length/2 - 1, rec.coord.Y - 1});
        cout << c;
        c = 180;
        cout << c;
    }    
}

void play(int height){
    
    char button;
    Rect temp_rect, null_rect[3];
    int temp_len = length, pointer = 0, remember_pointer = 0, exit_pointer = 0;
    bool finish = false, enter_pressed = false;
    COORD temp_coord = pyramid_null_coord, temp_hinge = hinges[0];
    bool rect_is_down = false;
    bool win = false;
    steps = 0;

    for(int i = 0; i < 3; i++){
        null_rect[i].color = DEF;
        null_rect[i].coord = {pyramid_null_coord.X + i*length + 2*i, pyramid_null_coord.Y};
        null_rect[i].select = true;
        null_rect[i].length = length;
    }

    pyramids[0].resize(height);
    pyramids[1].resize(0);
    pyramids[2].resize(0);

    for(int i = 0; i < height - 1; i++){
        temp_rect.length = temp_len;
        temp_len -= 4;
        temp_rect.coord = temp_coord;
        temp_coord.X += 2;
        temp_coord.Y -= 2;
        temp_rect.select = false;
        temp_rect.color = 16*color;
        if(++color == 6)color = 9;
        if(color == 15) color = 1;
        temp_rect.bottom = false;
        pyramids[0][i] = temp_rect;
        draw_rectangle(pyramids[0][i]);
        temp_hinge.Y -= 2;
    }

        temp_rect.length = temp_len;
        temp_len -= 4;
        temp_rect.coord = temp_coord;
        temp_coord.X += 2;
        temp_coord.Y -= 2;
        temp_rect.select = true;
        temp_rect.color = 16*color;
        if(++color == 6)color = 9;
        if(color == 15) color = 1;
        temp_rect.bottom = false;
        pyramids[0][height - 1] = temp_rect;
        draw_rectangle(pyramids[0][height - 1]);
        temp_hinge.Y -= 2;

    draw_table();



    // оставшаяся часть стержня 1
    char c1 = 195, c2 = 180;
    for(; temp_hinge.Y > hinge_top_Y; temp_hinge.Y--){
        SetConsoleCursorPosition(h, temp_hinge);
        cout << c1 << c2;
    }
    SetConsoleCursorPosition(h, temp_hinge);
    c1 = 218, c2 = 191;
    cout << c1 << c2;

    // стержни 2 и 3
    temp_hinge = hinges[1];
    for(int i = 0; i < 2; i++){
        c1 = 195;
        c2 = 180;
        for(; temp_hinge.Y > hinge_top_Y; temp_hinge.Y--){
            SetConsoleCursorPosition(h, temp_hinge);
            cout << c1 << c2;
        }
        SetConsoleCursorPosition(h, temp_hinge);
        c1 = 218;
        c2 = 191;
        cout << c1 << c2;
        temp_hinge = hinges[2];
    }
    temp_hinge = hinges[0];

    do{
        button = getch();
        switch (button){
            case LEFT:
            remember_pointer = pointer;
            if(pyramids[remember_pointer].empty()){
                null_rect[remember_pointer].select = false;
                destroy_rectangle(null_rect[remember_pointer]);
                if(--pointer == -1) pointer = 2;
                if(pyramids[pointer].empty()){
                    null_rect[pointer].select = true;
                    draw_rectangle(null_rect[pointer]);
                }else{
                    null_rect[remember_pointer].select = false;
                    destroy_rectangle(null_rect[remember_pointer]);
                    pyramids[pointer].back().select = true;
                    draw_rectangle(pyramids[pointer].back());
                }
            }else{
                if(--pointer == -1) pointer = 2;
                pyramids[remember_pointer].back().select = false;
                draw_rectangle(pyramids[remember_pointer].back());
                if(pyramids[pointer].empty()){
                    null_rect[pointer].select = true;
                    draw_rectangle(null_rect[pointer]);
                }else{
                    pyramids[pointer].back().select = true;
                    draw_rectangle(pyramids[pointer].back());
                }
            }
            break;
            case RIGHT:
            remember_pointer = pointer;
            if(pyramids[remember_pointer].empty()){
                null_rect[remember_pointer].select = false;
                destroy_rectangle(null_rect[remember_pointer]);
                if(++pointer == 3) pointer = 0;
                if(pyramids[pointer].empty()){
                    null_rect[pointer].select = true;
                    draw_rectangle(null_rect[pointer]);
                }else{
                    null_rect[remember_pointer].select = false;
                    destroy_rectangle(null_rect[remember_pointer]);
                    pyramids[pointer].back().select = true;
                    draw_rectangle(pyramids[pointer].back());
                }
            }else{
                if(++pointer == 3) pointer = 0;
                pyramids[remember_pointer].back().select = false;
                draw_rectangle(pyramids[remember_pointer].back());
                if(pyramids[pointer].empty()){
                    null_rect[pointer].select = true;
                    draw_rectangle(null_rect[pointer]);
                }else{
                    pyramids[pointer].back().select = true;
                    draw_rectangle(pyramids[pointer].back());
                }
            }
            break;
            case ENTER:
            if(pyramids[pointer].empty()){
                cout << "\a";
                break;
            }
            remember_pointer = pointer;
            temp_rect = pyramids[remember_pointer].back();
            pyramids[remember_pointer].pop_back();
            destroy_rectangle(temp_rect);
            temp_rect.coord.Y = hinge_top_Y - 1;
            draw_rectangle(temp_rect);
            rect_is_down = false;
            do{
                button = getch();
                switch(button){
                    case LEFT:
                    if(--pointer == -1) pointer = 2;
                    destroy_rectangle(temp_rect);
                    temp_rect.coord.X = hinges[pointer].X - temp_rect.length/2 + 1;
                    draw_rectangle(temp_rect);    
                    break;
                    case RIGHT:
                    if(++pointer == 3) pointer = 0;
                    destroy_rectangle(temp_rect);
                    temp_rect.coord.X = hinges[pointer].X - temp_rect.length/2 + 1;
                    draw_rectangle(temp_rect);
                    break;
                    case ENTER:
                    if(pyramids[pointer].empty()){
                        destroy_rectangle(temp_rect);
                        temp_rect.coord.Y = pyramid_null_coord.Y;
                        draw_rectangle(temp_rect); 
                        pyramids[pointer].push_back(temp_rect);
                        rect_is_down = true;
                    }else if(temp_rect.length < pyramids[pointer].back().length){
                        destroy_rectangle(temp_rect);
                        temp_rect.coord.Y = pyramids[pointer].back().coord.Y - 2;
                        draw_rectangle(temp_rect); 
                        pyramids[pointer].push_back(temp_rect);
                        rect_is_down = true;
                    }else if(temp_rect.length > pyramids[pointer].back().length) cout << "\a";
                    break;
                    case ESC:
                    system("cls");
                    drawbox();
                    draw_exit(exit_pointer);
                    do{
                        button = getch();
                        switch(button){
                            case LEFT:
                            if(--exit_pointer == -1) exit_pointer = 1;
                            draw_exit(exit_pointer);
                            break;
                            case RIGHT:
                            if(++exit_pointer == 2) exit_pointer = 0;
                            draw_exit(exit_pointer);
                            break;
                            case ENTER:
                            if(exit_pointer == 1){
                                system("cls");
                                drawbox();
                                int i = 0;
                                do{
                                    temp_hinge = hinges[i];
                                    for(vector <Rect>::iterator it = pyramids[i].begin(); it != pyramids[i].end(); it++){
                                    temp_hinge.Y -= 2;
                                    draw_rectangle(*it);
                                    }
                                    c1 = 195;
                                    c2 = 180;
                                    for(; temp_hinge.Y > hinge_top_Y; temp_hinge.Y--){
                                        SetConsoleCursorPosition(h, temp_hinge);
                                        cout << c1 << c2;
                                    }
                                    SetConsoleCursorPosition(h, temp_hinge);
                                    c1 = 218, c2 = 191;
                                    cout << c1 << c2;
                                    i++;
                                }while(i != 3);
                                draw_rectangle(temp_rect);
                                draw_table();
                                enter_pressed = true;
                            }else if(exit_pointer == 0){
                                finish = true;
                                system("cls");
                                drawbox();
                                enter_pressed = true;
                            }
                            break;
                        }
                    }while(!enter_pressed);
                    enter_pressed = false;
                    break;
                }
            }while(!rect_is_down && !finish);
            rect_is_down = false;
            steps++;
            if(pyramids[0].empty() && pyramids[3 - pointer].empty()) win = true;
            break;
            case ESC:
            system("cls");
            drawbox();
            draw_exit(exit_pointer);
            do{
                button = getch();
                switch(button){
                    case LEFT:
                    if(--exit_pointer == -1) exit_pointer = 1;
                    draw_exit(exit_pointer);
                    break;
                    case RIGHT:
                    if(++exit_pointer == 2) exit_pointer = 0;
                    draw_exit(exit_pointer);
                    break;
                    case ENTER:
                    if(exit_pointer == 1){
                        system("cls");
                        drawbox();
                        int i = 0;
                        do{
                            temp_hinge = hinges[i];
                            for(vector <Rect>::iterator it = pyramids[i].begin(); it != pyramids[i].end(); it++){
                            temp_hinge.Y -= 2;
                            draw_rectangle(*it);
                            }
                            c1 = 195;
                            c2 = 180;
                            for(; temp_hinge.Y > hinge_top_Y; temp_hinge.Y--){
                                SetConsoleCursorPosition(h, temp_hinge);
                                cout << c1 << c2;
                            }
                            SetConsoleCursorPosition(h, temp_hinge);
                            c1 = 218, c2 = 191;
                            cout << c1 << c2;
                            i++;
                        }while(i != 3);
                        draw_table();
                        enter_pressed = true;
                    }else if(exit_pointer == 0){
                        finish = true;
                        system("cls");
                        drawbox();
                        enter_pressed = true;
                    }
                }
            }while(!enter_pressed);
            enter_pressed = false;
            break;
        }
    }while(!win && !finish);

    if(win){
        system("cls");
        drawbox();
        if(steps == pow(2,height) - 1){
            temp_coord.Y = center.Y;
            temp_coord.X = center.X - strlen("Congratulations!")/2;
            SetConsoleCursorPosition(h, temp_coord);
            cout << "Congratulations!";
            temp_coord.Y++;
            temp_coord.X = center.X - strlen("You finished in minimum number of steps - xx. Well done!")/2;
            SetConsoleCursorPosition(h, temp_coord);
            cout << "You finished in minimum number of steps - " << steps << ". Well done!";
        }else if(steps > pow(2,height) - 1){
            temp_coord.Y = center.Y - 2;
            temp_coord.X = center.X - strlen("Congratulations!")/2;
            SetConsoleCursorPosition(h, temp_coord);
            cout << "Congratulations!";
            temp_coord.Y++;
            temp_coord.X = center.X - strlen("You finished in xx steps. Well done!")/2;
            SetConsoleCursorPosition(h, temp_coord);
            cout << "You finished in " << steps << " steps. Well done!";
            temp_coord.Y++;
            temp_coord.X = center.X - strlen("The minimum number of steps for this height is xx. Try to do better next time!")/2;
            SetConsoleCursorPosition(h, temp_coord);
            cout << "The minimum number of steps for this height is " << pow(2, height) - 1 << ". Try to do better next time!";
        }
        temp_coord.Y++;
        temp_coord.X = center.X - strlen("OK")/2;
        SetConsoleCursorPosition(h, temp_coord);
        SetConsoleTextAttribute(h, MENUATTR);
        cout << "OK";
        SetConsoleTextAttribute(h, DEF);
        do{
            button = getch();
            switch(button){
            case ENTER:
            break;
            case ESC:
            break;
        }
        }while(button != ESC && button != ENTER);
    }    
}

void submenu(){

    draw_submenu();
    draw_numbers(maxheight, 0);

    char button;
    int exit = 1;

    int position = 0;

    do{
        button = getch();
        switch(button){
            case LEFT: 
            position--;
            if(position == -1) position = maxheight - 1;
            draw_numbers(maxheight, position);
            break;
            case RIGHT:
            position++;
            if(position == maxheight) position = 0;
            draw_numbers(maxheight, position);         
            break;
            case ENTER:
            system("cls");
            drawbox();
            play(position + 1);
            system("cls");
            drawbox();
            exit = 0;
            break;
            case ESC: 
            exit = 0;
            system("cls");
            drawbox();
            break;
        }
    }while(exit);
}

void how_to_play(){
    bool exit_ = false;
    char button;
    DWORD lp;

    system("cls");
    drawbox();
    SetConsoleCursorPosition(h, {cs.srWindow.Right/3 - strlen("Press \"Play\" in the menu")/2, center.Y});
    cout << "Press \"Play\" in the menu";
    SetConsoleTextAttribute(h, MENUATTR);
    SetConsoleCursorPosition(h, {cs.srWindow.Right/3 - strlen("Next")/2, center.Y + 1});
    cout << "Next";
    SetConsoleTextAttribute(h, DEF);

    for(int i = 0, j = -1, k = 0; i < 4; i++, j++, k++){
        SetConsoleCursorPosition(h, {cs.srWindow.Right - cs.srWindow.Right/3 - strlen(menu[i])/2, center.Y + j});
        if(k == 0){
            SetConsoleTextAttribute(h, MENUATTR);
            cout << menu[i] << ' ';
            SetConsoleTextAttribute(h, DEF);
        }else cout << menu[i] << ' ';

    }

    SetConsoleCursorPosition(h, {cs.srWindow.Right/3 + strlen("Next")/2, center.Y + 1});

    do{
        button = getch();
        switch(button){
        case ENTER:
        break;
        case ESC:
        exit_ = true;
        break;
        }
    }while(button != ENTER && button != ESC);
  
    if(exit_) return;

    FillConsoleOutputCharacter(h, ' ', strlen("Press \"Play\" in the menu"), {cs.srWindow.Right/3 - strlen("Press \"Play\" in the menu")/2, center.Y}, &lp);
    if(lp != strlen("Press \"Play\" in the menu")){
        system("cls");
        drawbox();
        SetConsoleCursorPosition(h, {center.X - strlen("Sorry, some mistake")/2, center.Y});
        cout << "Sorry, some mistake";
        Sleep(1500);
        exit(0);
    }

    for(int i = 0, j = -1; i < 4; i++, j++){
        FillConsoleOutputCharacter(h, ' ', strlen(menu[i]), {cs.srWindow.Right - cs.srWindow.Right/3 - strlen(menu[i])/2, center.Y + j}, &lp);
        if(lp != strlen(menu[i])){
            system("cls");
            drawbox();
            SetConsoleCursorPosition(h, {center.X - strlen("Sorry, some mistake")/2, center.Y});
            cout << "Sorry, some mistake";
            Sleep(1500);
            exit(0);
        }
    }

    SetConsoleCursorPosition(h, {cs.srWindow.Right/3 - strlen("Choose the height of the start-pyramid")/2, center.Y});
    cout << "Choose the height of the start-pyramid";
    SetConsoleTextAttribute(h, MENUATTR);
    SetConsoleCursorPosition(h, {cs.srWindow.Right/3 - strlen("Next")/2, center.Y + 1});
    cout << "Next";
    SetConsoleTextAttribute(h, DEF);

    SetConsoleCursorPosition(h, {cs.srWindow.Right - cs.srWindow.Right/3 - strlen("What height do you want?")/2, center.Y});

    cout << "What height do you want?";

    SetConsoleCursorPosition(h, {cs.srWindow.Right - cs.srWindow.Right/3 - (maxheight - 1), center.Y + 1});
    for(int i = 0; i < maxheight; i++){
        if(i == maxheight - 1){
            SetConsoleTextAttribute(h, MENUATTR);
            cout << i + 1;
            SetConsoleTextAttribute(h, DEF);
        }else cout << i + 1;
        cout << ' ';
    }

    SetConsoleCursorPosition(h, {cs.srWindow.Right/3 + strlen("Next")/2, center.Y + 1});

    do{
        button = getch();
        switch(button){
        case ENTER:
        break;
        case ESC:
        exit_ = true;
        break;
        }
    }while(button != ENTER && button != ESC);
    
    if(exit_) return;

    FillConsoleOutputCharacter(h, ' ', strlen("Choose the height of the start-pyramid"), {cs.srWindow.Right/3 - strlen("Choose the height of the start-pyramid")/2, center.Y}, &lp);
    if(lp != strlen("Choose the height of the start-pyramid")){
        system("cls");
        drawbox();
        SetConsoleCursorPosition(h, {center.X - strlen("Sorry, some mistake")/2, center.Y});
        cout << "Sorry, some mistake";
        Sleep(1500);
        exit(0);
    }

    FillConsoleOutputCharacter(h, ' ', strlen("What height do you want?"), {cs.srWindow.Right - cs.srWindow.Right/3 - strlen("What height do you want?")/2, center.Y}, &lp);
    if(lp != strlen("What height do you want?")){
        system("cls");
        drawbox();
        SetConsoleCursorPosition(h, {center.X - strlen("Sorry, some mistake")/2, center.Y});
        cout << "Sorry, some mistake";
        Sleep(1500);
        exit(0);
    }
    FillConsoleOutputCharacter(h, ' ', 2*maxheight, {cs.srWindow.Right - cs.srWindow.Right/3 - (maxheight - 1), center.Y + 1}, &lp);
    if(lp != 2*maxheight){
        system("cls");
        drawbox();
        SetConsoleCursorPosition(h, {center.X - strlen("Sorry, some mistake")/2, center.Y});
        cout << "Sorry, some mistake";
        Sleep(1500);
        exit(0);
    }

    SetConsoleCursorPosition(h, {cs.srWindow.Right/3 - strlen("The pyramid of chosen height will be drawn")/2, center.Y});
    cout << "The pyramid of chosen height will be drawn";
    SetConsoleTextAttribute(h, MENUATTR);
    SetConsoleCursorPosition(h, {cs.srWindow.Right/3 - strlen("Next")/2, center.Y + 1});
    cout << "Next";
    SetConsoleTextAttribute(h, DEF);

    vector <Rect> pyramid;
    Rect temp_rect;
    int len = length;
    COORD temp_hinge = hinges[2];

    if(maxheight < 2){
        system("cls");
        drawbox();
        SetConsoleCursorPosition(h, {center.X - strlen("Sorry, some mistake")/2, center.Y});
        cout << "Sorry, some mistake";
        Sleep(1500);
        exit(0);
    } 
    for(int i = 0; i < maxheight - 1; i++){
        temp_rect.length = len;
        len -= 4;
        temp_rect.coord = {pyramid_null_coord.X + 2*length + 4 + 2*i, pyramid_null_coord.Y - 2*i};
        temp_hinge.Y -= 2;
        temp_rect.select = false;
        temp_rect.color = 16*color;
        if(++color == 7)color = 9;
        if(color == 15) color = 1;
        pyramid.push_back(temp_rect);
        draw_rectangle(pyramid.back());
    }

    temp_rect.length = len;
    len -= 4;
    temp_rect.coord = {pyramid_null_coord.X + 2*length + 4 + 2*(maxheight - 1), pyramid_null_coord.Y - 2*(maxheight - 1)};
    temp_hinge.Y -= 2;
    temp_rect.select = true;
    temp_rect.color = 16*color;
    if(++color == 7)color = 9;
    if(color == 15) color = 1;
    pyramid.push_back(temp_rect);
    draw_rectangle(pyramid.back());

    char c1 = 195, c2 = 180;
    for(; temp_hinge.Y > hinge_top_Y; temp_hinge.Y--){
        SetConsoleCursorPosition(h, temp_hinge);
        cout << c1 << c2;
    }
    SetConsoleCursorPosition(h, temp_hinge);
    c1 = 218, c2 = 191;
    cout << c1 << c2;

    SetConsoleCursorPosition(h, {cs.srWindow.Right/3 + strlen("Next")/2, center.Y + 1});

    do{
        button = getch();
        switch(button){
            case ENTER:
            break;
            case ESC:
            exit_ = true;
            break;
        }
    }while(button != ENTER && button != ESC);

    if(exit_) return;
    
    FillConsoleOutputCharacter(h, ' ', strlen("The pyramid of chosen height will be drawn"), {cs.srWindow.Right/3 - strlen("The pyramid of chosen height will be drawn")/2, center.Y}, &lp);
    if(lp != strlen("The pyramid of chosen height will be drawn")){
        system("cls");
        drawbox();
        SetConsoleCursorPosition(h, {center.X - strlen("Sorry, some mistake")/2, center.Y});
        cout << "Sorry, some mistake";
        Sleep(1500);
        exit(0);
    }

    if(exit_) return;

    SetConsoleCursorPosition(h, {cs.srWindow.Right/3 - strlen("Press \"Enter\" to raise the top unit of the pyramid")/2, center.Y});
    cout << "Press \"Enter\" to raise the top unit of the pyramid";

    destroy_rectangle(pyramid.back());
    temp_rect = pyramid.back();
    pyramid.back().coord.Y = hinge_top_Y - 1;
    draw_rectangle(pyramid.back());
    SetConsoleCursorPosition(h, {cs.srWindow.Right/3 + strlen("Next")/2, center.Y + 1});
    Sleep(800);
    destroy_rectangle(pyramid.back());
    pyramid.back().coord.Y = temp_rect.coord.Y;
    draw_rectangle(pyramid.back());
    SetConsoleCursorPosition(h, {cs.srWindow.Right/3 + strlen("Next")/2, center.Y + 1});
    Sleep(800);
    destroy_rectangle(pyramid.back());
    pyramid.back().coord.Y = hinge_top_Y - 1;
    temp_rect = pyramid.back();
    pyramid.pop_back();
    draw_rectangle(temp_rect);

    SetConsoleCursorPosition(h, {cs.srWindow.Right/3 + strlen("Next")/2, center.Y + 1});

    do{
        button = getch();
        switch(button){
            case ENTER:
            break;
            case ESC:
            exit_ = true;
            break;
        }
    }while(button != ENTER && button != ESC);

    if(exit_) return;

    FillConsoleOutputCharacter(h, ' ', strlen("Press \"Enter\" to raise the top unit of the pyramid"), {cs.srWindow.Right/3 - strlen("Press \"Enter\" to raise the top unit of the pyramid")/2, center.Y}, &lp);
    if(lp != strlen("Press \"Enter\" to raise the top unit of the pyramid")){
        system("cls");
        drawbox();
        SetConsoleCursorPosition(h, {center.X - strlen("Sorry, some mistake")/2, center.Y});
        cout << "Sorry, some mistake";
        Sleep(1500);
        exit(0);
    }

    SetConsoleCursorPosition(h, {cs.srWindow.Right/3 - strlen("Use arrow keys on your keyboard to move the unit left or right")/2, center.Y});
    cout << "Use arrow keys on your keyboard to move the unit left or right";

    SetConsoleCursorPosition(h, {cs.srWindow.Right/3 + strlen("Next")/2, center.Y + 1});

    destroy_rectangle(temp_rect);
    temp_rect.coord.X -= temp_rect.length;
    draw_rectangle(temp_rect);
    SetConsoleCursorPosition(h, {cs.srWindow.Right/3 + strlen("Next")/2, center.Y + 1});
    Sleep(800);
    destroy_rectangle(temp_rect);
    temp_rect.coord.X += temp_rect.length;
    draw_rectangle(temp_rect);
    SetConsoleCursorPosition(h, {cs.srWindow.Right/3 + strlen("Next")/2, center.Y + 1});
    Sleep(800);
    destroy_rectangle(temp_rect);
    temp_rect.coord.X += temp_rect.length;
    draw_rectangle(temp_rect);
    SetConsoleCursorPosition(h, {cs.srWindow.Right/3 + strlen("Next")/2, center.Y + 1});
    Sleep(800);
    destroy_rectangle(temp_rect);
    temp_rect.coord.X -= temp_rect.length;
    draw_rectangle(temp_rect);

    SetConsoleCursorPosition(h, {cs.srWindow.Right/3 + strlen("Next")/2, center.Y + 1});

    do{
        button = getch();
        switch(button){
            case ENTER:
            break;
            case ESC:
            exit_ = true;
            break;
        }
    }while(button != ESC && button != ENTER);

    if(exit_) return;

    FillConsoleOutputCharacter(h, ' ', strlen("Use arrow keys on your keyboard to move the unit left or right"), {cs.srWindow.Right/3 - strlen("Use arrow keys on your keyboard to move the unit left or right")/2, center.Y}, &lp);
    if(lp != strlen("Use arrow keys on your keyboard to move the unit left or right")){
        system("cls");
        drawbox();
        SetConsoleCursorPosition(h, {center.X - strlen("Sorry, some mistake")/2, center.Y});
        cout << "Sorry, some mistake";
        Sleep(1500);
        exit(0);
    }
    
    SetConsoleCursorPosition(h, {cs.srWindow.Right/3 - strlen("Press \"Enter\" if you want to put down the unit")/2, center.Y});
    cout << "Press \"Enter\" if you want to put down the unit";

    SetConsoleCursorPosition(h, {cs.srWindow.Right/3 + strlen("Next")/2, center.Y + 1});

    destroy_rectangle(temp_rect);
    temp_rect.coord.Y = pyramid.back().coord.Y - 2;
    draw_rectangle(temp_rect);
    Sleep(800);
    SetConsoleCursorPosition(h, {cs.srWindow.Right/3 + strlen("Next")/2, center.Y + 1});
    destroy_rectangle(temp_rect);
    temp_rect.coord.Y = hinge_top_Y - 1;
    draw_rectangle(temp_rect);
    Sleep(800);
    SetConsoleCursorPosition(h, {cs.srWindow.Right/3 + strlen("Next")/2, center.Y + 1});
    destroy_rectangle(temp_rect);
    temp_rect.coord.Y = pyramid.back().coord.Y - 2;
    draw_rectangle(temp_rect);

    pyramid.push_back(temp_rect);

    SetConsoleCursorPosition(h, {cs.srWindow.Right/3 + strlen("Next")/2, center.Y + 1});

    do{
        button = getch();
        switch(button){
            case ENTER:
            break;
            case ESC:
            exit_ = true;
            break;
        }
    }while(button != ESC && button != ENTER);

    if(exit_) return;
    
    FillConsoleOutputCharacter(h, ' ', strlen("Press \"Enter\" if you want to put down the unit"), {cs.srWindow.Right/3 - strlen("Press \"Enter\" if you want to put down the unit")/2, center.Y}, &lp);
    if(lp != strlen("Press \"Enter\" if you want to put down the unit")){
        system("cls");
        drawbox();
        SetConsoleCursorPosition(h, {center.X - strlen("Sorry, some mistake")/2, center.Y});
        cout << "Sorry, some mistake";
        Sleep(1500);
        exit(0);
    }

    for(vector <Rect>:: iterator iter = pyramid.begin(); iter != pyramid.end(); iter++){
        destroy_rectangle(*iter);
    }

    SetConsoleCursorPosition(h, {cs.srWindow.Right/3 - strlen("Remember that you can not put a longer unit onto a shorter one")/2, center.Y});
    cout << "Remember that you can not put a longer unit onto a shorter one";

    SetConsoleCursorPosition(h, {cs.srWindow.Right/3 - strlen("Next")/2, center.Y + 1});
    SetConsoleTextAttribute(h, MENUATTR);
    cout << "Next";
    SetConsoleTextAttribute(h, DEF);

    SetConsoleCursorPosition(h, {cs.srWindow.Right/3 + strlen("Next")/2, center.Y + 1});

    pyramid.at(maxheight - 2).select = true;
    pyramid.at(maxheight - 2).coord.Y = hinge_top_Y - 1;
    temp_rect = pyramid.at(maxheight - 2);
    draw_rectangle(pyramid.at(maxheight - 2));
    pyramid.at(maxheight - 2) = pyramid.back();

    pyramid.at(maxheight - 2).coord.Y += 2;
    pyramid.at(maxheight - 2).select = false;
    
    pyramid.pop_back();

    for(vector <Rect>:: iterator iter = pyramid.begin(); iter != pyramid.end(); iter++){
        draw_rectangle(*iter);
    }

    c1 = 195;
    c2 = 180;

    for(int i = pyramid.back().coord.Y - 2; i > hinge_top_Y; i--){
        SetConsoleCursorPosition(h, {hinges[2].X, i});
        cout << c1 << c2;
    }

    SetConsoleCursorPosition(h, {hinges[2].X, hinge_top_Y});

    c1 = 218;
    c2 = 191;
    cout << c1 << c2;

    SetConsoleCursorPosition(h, {cs.srWindow.Right/3 + strlen("Next")/2, center.Y + 1});

    do{
        button = getch();
        switch(button){
            case ENTER:
            break;
            case ESC:
            exit_ = true;
            break;
        }
    }while(button != ESC && button != ENTER);

    if(exit_) return;

    FillConsoleOutputCharacter(h, ' ', strlen("Remember that you can not put a longer unit onto a shorter one"), {cs.srWindow.Right/3 - strlen("Remember that you can not put a longer unit onto a shorter one")/2, center.Y}, &lp);
    if(lp != strlen("Remember that you can not put a longer unit onto a shorter one")){
        system("cls");
        drawbox();
        SetConsoleCursorPosition(h, {center.X - strlen("Sorry, some mistake")/2, center.Y});
        cout << "Sorry, some mistake";
        Sleep(1500);
        exit(0);
    }

    FillConsoleOutputCharacter(h, ' ', strlen("Next"), {cs.srWindow.Right/3 - strlen("Next")/2, center.Y + 1}, &lp);
    if(lp != strlen("Next")){
        system("cls");
        drawbox();
        SetConsoleCursorPosition(h, {center.X - strlen("Sorry, some mistake")/2, center.Y});
        cout << "Sorry, some mistake";
        Sleep(1500);
        exit(0);
    }

    SetConsoleCursorPosition(h, {cs.srWindow.Right/3 - strlen("If you try to do it, you will hear the sound")/2, center.Y});
    cout << "If you try to do it, you will hear the sound";

    SetConsoleCursorPosition(h, {cs.srWindow.Right/3 - strlen("Hear the sound")/2, center.Y + 1});
    SetConsoleTextAttribute(h, MENUATTR);
    cout << "Hear the sound";
    SetConsoleTextAttribute(h, DEF);

    do{
        button = getch();
        switch(button){
            case ENTER:
            cout << "\a";
            break;
            case ESC:
            exit_ = true;
            break;
        }
    }while(button != ENTER && button != ESC);

    if(exit_) return;

    FillConsoleOutputCharacter(h, ' ', strlen("Hear the sound"), {cs.srWindow.Right/3 - strlen("Hear the sound")/2, center.Y + 1}, &lp);
    if(lp != strlen("Hear the sound")){
        system("cls");
        drawbox();
        SetConsoleCursorPosition(h, {center.X - strlen("Sorry, some mistake")/2, center.Y});
        cout << "Sorry, some mistake";
        Sleep(1500);
        exit(0);
    }

    SetConsoleCursorPosition(h, {cs.srWindow.Right/3 - strlen("Next")/2, center.Y + 1});
    SetConsoleTextAttribute(h, MENUATTR);
    cout << "Next";
    SetConsoleTextAttribute(h, DEF);

    do{
        button = getch();
        switch(button){
            case ENTER:
            break;
            case ESC:
            exit_ = true;
            break;
        }
    }while(button != ESC && button != ENTER);

    if(exit_) return;

    FillConsoleOutputCharacter(h, ' ', strlen("If you try to do it, you will hear the sound"), {cs.srWindow.Right/3 - strlen("If you try to do it, you will hear the sound")/2, center.Y}, &lp);
    if(lp != strlen("If you try to do it, you will hear the sound")){
        system("cls");
        drawbox();
        SetConsoleCursorPosition(h, {center.X - strlen("Sorry, some mistake")/2, center.Y});
        cout << "Sorry, some mistake";
        Sleep(1500);
        exit(0);
    }

    destroy_rectangle(temp_rect);
    destroy_rectangle(pyramid.back());

    pyramid.push_back(pyramid.back());
    pyramid.back().coord.Y -= 2;
    pyramid.back().select = true;
    pyramid.at(maxheight - 3) = temp_rect;
    pyramid.at(maxheight - 3).coord.Y = pyramid.back().coord.Y + 2;
    pyramid.at(maxheight - 3).select = false;

    draw_rectangle(pyramid.at(maxheight - 3));
    draw_rectangle(pyramid.back());

    SetConsoleCursorPosition(h, {cs.srWindow.Right/3 - strlen("To win you have to move the pyramid to another peg.Good luck!")/2, center.Y});
    cout << "To win you have to move the pyramid to another peg. Good luck!";

    SetConsoleCursorPosition(h, {cs.srWindow.Right/3 + strlen("Next")/2, center.Y + 1});

    do{
        button = getch();
        switch(button){
            case ENTER:
            break;
            case ESC:
            exit_ = true;
            break;
        }
    }while(button != ESC && button != ENTER);
}

void move(int from, int to){
    COORD tmp = {center.X + strlen("Number of steps: ")/2, hinge_top_Y - 4}, tmp1 = {center.X + strlen("Number of steps: ")/2 + 1, hinge_top_Y - 4};
    if(steps >= 10) tmp1.X += 1;

    destroy_rectangle(pyramids[from].back());
    SetConsoleCursorPosition(h, tmp1);
    pyramids[from].back().coord.Y = hinge_top_Y - 1;
    draw_rectangle(pyramids[from].back());
    SetConsoleCursorPosition(h, tmp1);
    Sleep(800);

    if(from == 2 || from == 0){
        destroy_rectangle(pyramids[from].back());
        SetConsoleCursorPosition(h, tmp1);
        pyramids[from].back().coord.X = hinges[1].X - pyramids[from].back().length/2 + 1;
        draw_rectangle(pyramids[from].back());
        SetConsoleCursorPosition(h, tmp1);
        Sleep(800);
        if(to == 1 && pyramids[1].empty()){
            destroy_rectangle(pyramids[from].back());
            SetConsoleCursorPosition(h, tmp1);
            pyramids[from].back().coord.Y = pyramid_null_coord.Y;
            draw_rectangle(pyramids[from].back());
            SetConsoleCursorPosition(h, tmp);
            cout << ++steps;
            Sleep(800);
            pyramids[1].push_back(pyramids[from].back());
            pyramids[from].pop_back();
        }else if(to == 1 && !pyramids[1].empty()){
            destroy_rectangle(pyramids[from].back());
            SetConsoleCursorPosition(h, tmp1);
            pyramids[from].back().coord.Y = pyramids[1].back().coord.Y - 2;
            draw_rectangle(pyramids[from].back());
            SetConsoleCursorPosition(h, tmp);
            cout << ++steps;
            Sleep(800);
            pyramids[1].push_back(pyramids[from].back());
            pyramids[from].pop_back();
        }else{
            destroy_rectangle(pyramids[from].back());
            SetConsoleCursorPosition(h, tmp1);
            pyramids[from].back().coord.X = hinges[2 - from].X - pyramids[from].back().length/2 + 1;
            draw_rectangle(pyramids[from].back());
            SetConsoleCursorPosition(h, tmp1);
            Sleep(800);
            if(pyramids[2 - from].empty()){
                destroy_rectangle(pyramids[from].back());
                SetConsoleCursorPosition(h, tmp1);
                pyramids[from].back().coord.Y = pyramid_null_coord.Y;
                draw_rectangle(pyramids[from].back());
                SetConsoleCursorPosition(h, tmp);
                cout << ++steps;
                Sleep(800);
                pyramids[2 - from].push_back(pyramids[from].back());
                pyramids[from].pop_back();
            }else{
                destroy_rectangle(pyramids[from].back());
                SetConsoleCursorPosition(h, tmp1);
                pyramids[from].back().coord.Y = pyramids[2 - from].back().coord.Y - 2;
                draw_rectangle(pyramids[from].back());
                SetConsoleCursorPosition(h, tmp);
                cout << ++steps;
                Sleep(800);
                pyramids[2 - from].push_back(pyramids[from].back());
                pyramids[from].pop_back();
            }
        }
    }else{
        destroy_rectangle(pyramids[from].back());
        SetConsoleCursorPosition(h, tmp1);
        pyramids[from].back().coord.X = hinges[to].X - pyramids[from].back().length/2 + 1;
        draw_rectangle(pyramids[from].back());
        SetConsoleCursorPosition(h, tmp1);
        Sleep(800);
        destroy_rectangle(pyramids[from].back());
        SetConsoleCursorPosition(h, tmp1);
        if(pyramids[to].empty()){
            pyramids[from].back().coord.Y = pyramid_null_coord.Y;
            draw_rectangle(pyramids[from].back());
            SetConsoleCursorPosition(h, tmp);
            cout << ++steps;
            Sleep(800);
            pyramids[to].push_back(pyramids[from].back());
            pyramids[from].pop_back();
        }else if(!pyramids[to].empty()){
            pyramids[from].back().coord.Y = pyramids[to].back().coord.Y - 2;
            draw_rectangle(pyramids[from].back());
            SetConsoleCursorPosition(h, tmp);
            cout << ++steps;
            Sleep(800);
            pyramids[to].push_back(pyramids[from].back());
            pyramids[from].pop_back();
        }
    }
}

void hanoi(int height, int from, int to){
    if(height == 1) move(from, to);
    else{
        int temp = 3 - from - to;
        hanoi(height - 1, from, temp);
        move(from, to);
        hanoi(height - 1, temp, to);
    }
    return;
}

void correct_algorithm(int height){
    Rect temp_rect;
    int temp_len = length;
    char button;
    pyramids[0].resize(height);
    pyramids[1].resize(0);
    pyramids[2].resize(0);
    COORD temp_hinge = hinges[0], temp_coord = pyramid_null_coord;

    for(int i = 0; i < height; i++){
        temp_rect.length = temp_len;
        temp_len -= 4;
        temp_rect.coord = temp_coord;
        temp_coord.X += 2;
        temp_coord.Y -= 2;
        temp_rect.select = false;
        temp_rect.color = 16*color;
        if(++color == 6)color = 9;
        if(color == 15) color = 1;
        temp_rect.bottom = false;
        pyramids[0][i] = temp_rect;
        draw_rectangle(pyramids[0][i]);
        temp_hinge.Y -= 2;
    }

    draw_table();

    char c1 = 195, c2 = 180;
    for(; temp_hinge.Y > hinge_top_Y; temp_hinge.Y--){
        SetConsoleCursorPosition(h, temp_hinge);
        cout << c1 << c2;
    }
    SetConsoleCursorPosition(h, temp_hinge);
    c1 = 218, c2 = 191;
    cout << c1 << c2;

    // стержни 2 и 3
    temp_hinge = hinges[1];
    for(int i = 0; i < 2; i++){
        c1 = 195;
        c2 = 180;
        for(; temp_hinge.Y > hinge_top_Y; temp_hinge.Y--){
            SetConsoleCursorPosition(h, temp_hinge);
            cout << c1 << c2;
        }
        SetConsoleCursorPosition(h, temp_hinge);
        c1 = 218;
        c2 = 191;
        cout << c1 << c2;
        temp_hinge = hinges[2];
    }
    
    Sleep(300);

    SetConsoleCursorPosition(h, {center.X - strlen("Number of steps: xx")/2, hinge_top_Y - 4});
    cout << "Number of steps: 0";
    hanoi(height, 0, 1);

    SetConsoleCursorPosition(h, {center.X - strlen("OK")/2, hinge_top_Y - 3});
    SetConsoleTextAttribute(h, MENUATTR);
    cout << "OK";
    do{
        button = getch();
        switch(button){
            case ENTER:
            break;
            case ESC:
            break;
        }
    }while(button != ENTER && button != ESC);
    SetConsoleTextAttribute(h, DEF);
}

void watch_the_coorect_algoritm(){
    draw_submenu();
    draw_numbers(maxheight, 0);

    char button;
    int exit = 1;
    steps = 0;

    int position = 0;

    do{
        button = getch();
        switch(button){
            case LEFT: 
            position--;
            if(position == -1) position = maxheight - 1;
            draw_numbers(maxheight, position);
            break;
            case RIGHT:
            position++;
            if(position == maxheight) position = 0;
            draw_numbers(maxheight, position);         
            break;
            case ENTER:
            system("cls");
            drawbox();
            correct_algorithm(position + 1);
            system("cls");
            drawbox();
            exit = 0;
            break;
            case ESC: 
            exit = 0;
            system("cls");
            drawbox();
            break;
        }
    }while(exit);
}