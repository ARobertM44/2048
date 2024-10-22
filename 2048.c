#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* DISCLAIMER
  -pentru incadrarea propice a jocului in terminal nu este
   recomandat ca acesta sa fie zoomat prea mult
  -chiar daca bomba ar trebui sa se activeze dupa 2000 de puncte,
   pentru usurinta verificarilor am pus un target de doar 100 puncte
  -pentru verificarea ecranului de castig se poate modifica pe 
   linia 453 valoarea de 2048 in una mai usor de atins
  -de fun se poate modifica valoarea pe care trebuie sa o atinga k
   intr-una mai mica (sa zicem 5 sau 0 spre exemplu) pentru a pune 
   algoritmul in actiune pe linia 866*/

/* Aceasta functie introduce primele
   doua elemente random la inceputul jocului*/
void start(int m[][4]) {
    int i, j, x;
    /* facem rost de o valoare random generata de ceas
       cu ajutorul functiilor srand si rand*/
    srand(time(NULL));
    // % 4 folosit pentru a genera o linie de la 0 la 3
    i = rand() % 4;
    // % 4 folosit pentru a genera o coloana de la 0 la 3
    j = rand() % 4;
    // % 2 folosit pentru a genera 0 sau 1
    x = rand() % 2;
    if (x) {
        m[i][j] = 2;
    } else {
        m[i][j] = 4;
    }
    /* am generat primul numar, dar acum trebuie
       sa ne asiguram ca al doilea numar nu este generat
       tot pe pozitia primului, suprascriindu-l*/
    while (1) {
        i = rand() % 4;
        j = rand() % 4;
        /* se verifica daca celula este goala si
           se repeta pasii de la prima generare*/
        if (m[i][j] == 0) {
            x = rand() % 2;
            if (x) {
                m[i][j] = 2;
            } else {
                m[i][j] = 4;
            }
            break;
        }
    }
}
// aceasta functie creeaza o matrice plina de 0
void matrix(int m[][4]) {
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            m[i][j] = 0;
        }
    }
}
/* functia care misca toate celulele la dreapta,
   la apasarea de right arrow*/
void right(int m[][4], int *score, int *moves) {
    int i, j, aux, x, ok = 0, ok1, ok2, ok3, ok4;
    for (i = 0; i < 4; i++) {
        /* ok1 este folosit pentru a monitoriza daca
           casutele de pe pozitiile [i][2] si [i][3]
           au aceeasi valoare, adica s-au combinat*/
        ok1 = 1;
        /* ok2 este folosit pentru a monitoriza daca
           casutele de pe pozitiile [i][1] si [i][2]
           au aceeasi valoare, adica s-au combinat*/
        ok2 = 1;
        ok3 = 1;
        ok4 = 1;
        if(m[i][1] == 0){
            ok3 = 0;
        }
        /* practic ok1 si ok2 sunt folosite pentru a
           nu se crea combinari inlantuite cu aceeasi mutare*/
        // parcurgem matricea de la dreapta la stanga
        for (j = 2; j > -1; j--) {
            aux = j;
            /* verificam daca casuta din drepta casutei
               actuale este ocupata sau nu*/
            while (aux < 3 && m[i][aux + 1] == 0) {
                /* daca casuta din dreapta nu este ocupata,
                   iar casuta actuala este ocupata, inseamna ca
                   miscarea se poate produce cu succes*/
                if (m[i][aux] != 0) {
                    ok = 1;
                }
                // casuta viitoare ia valoarea casutei actuale
                m[i][aux + 1] = m[i][aux];
                // casuta actuala se goleste
                m[i][aux] = 0;
                // se repeta procesul
                aux++;
            }
            if (aux != 3 && m[i][aux + 1] != 0) {
                /* aici vom verifica daca casuta [i][2]
                   este identica cu casuta [i][3]*/
                if (j == 2 && m[i][aux] == m[i][aux + 1]) {
                    /* daca casuta actuala este ocupata inseamna
                       ca mutarea este valida si combinarea
                       se poate realiza cu succes*/
                    if (m[i][aux] != 0) {
                        ok = 1;
                    }
                    // combinarea propriu zisa
                    m[i][aux + 1] = 2 * m[i][aux + 1];
                    m[i][aux] = 0;
                    // scorul se modifica corespunzator
                    *score = *score + m[i][aux + 1];
                    ok1 = 0;
                }
                /* acum verificam identitatea dintre casutele [i][1] si [i][2]
                   daca casutele [i][2] si [i][3] nu s-au unit*/
                // procesul este asemanator cu cel de mai sus
                if (j == 1 && ok1 && m[i][aux] == m[i][aux + 1]) {
                    if (m[i][aux] != 0) {
                        ok = 1;
                    }
                    m[i][aux + 1] = 2 * m[i][aux + 1];
                    m[i][aux] = 0;
                    *score = *score + m[i][aux + 1];
                    ok2 = 0;
                }
                if(ok3 == 0 && ok1 == 0){
                    ok4 = 0;
                }
                /* verificare intre casutele [i][0] si [i][1] daca
                   casutele [i][1] si [i][2] nu s-au unit*/
                if (j == 0 && ok2 && ok4 && m[i][aux] == m[i][aux + 1]) {
                    if (m[i][aux] != 0) {
                        ok = 1;
                    }
                    m[i][aux + 1] = 2 * m[i][aux + 1];
                    m[i][aux] = 0;
                    *score = *score + m[i][aux + 1];
                }
            }
        }
    }
    /* daca mutarea este valida se va genera
       o noua casuta intr-o pozitie random*/
    if (ok) {
        (*moves)++;
        srand(time(NULL));
        while (1) {
            i = rand() % 4;
            j = rand() % 4;
            if (m[i][j] == 0) {
                x = rand() % 2;
                if (x) {
                    m[i][j] = 2;
                } else {
                    m[i][j] = 4;
                }
                break;
            }
        }
    }
}
/* functia left este extrem de asemanatoare cu functia right,
   doar ca citirea se va face de data aceasta de la stanga la dreapta,
   iar miscarea tuturor casutelor va fi spre stanga*/
void left(int m[][4], int *score, int *moves) {
    int i, j, aux, x, ok = 0, ok1, ok2, ok3, ok4;
    for (i = 0; i < 4; i++) {
        ok1 = 1;
        ok2 = 1;
        ok3 = 1;
        ok4 = 1;
        if(m[i][2] == 0){
            ok3 = 0;
        }
        for (j = 1; j < 4; j++) {
            aux = j;
            while (aux > 0 && m[i][aux - 1] == 0) {
                if (m[i][aux] != 0) {
                    ok = 1;
                }
                m[i][aux - 1] = m[i][aux];
                m[i][aux] = 0;
                aux--;
            }
            if (aux != 0 && m[i][aux - 1] != 0) {
                if (j == 1 && m[i][aux] == m[i][aux - 1]) {
                    if (m[i][aux] != 0) {
                        ok = 1;
                    }
                    m[i][aux - 1] = 2 * m[i][aux - 1];
                    m[i][aux] = 0;
                    *score = *score + m[i][aux - 1];
                    ok1 = 0;
                }
                if (j == 2 && ok1 && m[i][aux] == m[i][aux - 1]) {
                    if (m[i][aux] != 0) {
                        ok = 1;
                    }
                    m[i][aux - 1] = 2 * m[i][aux - 1];
                    m[i][aux] = 0;
                    *score = *score + m[i][aux - 1];
                    ok2 = 0;
                }
                if(ok3 == 0 && ok1 == 0){
                    ok4 = 0;
                }
                if (j == 3 && ok2 && ok4 && m[i][aux] == m[i][aux - 1]) {
                    if (m[i][aux] != 0) {
                        ok = 1;
                    }
                    m[i][aux - 1] = 2 * m[i][aux - 1];
                    m[i][aux] = 0;
                    *score = *score + m[i][aux - 1];
                }
            }
        }
    }
    if (ok) {
        (*moves)++;
        srand(time(NULL));
        while (1) {
            i = rand() % 4;
            j = rand() % 4;
            if (m[i][j] == 0) {
                x = rand() % 2;
                if (x) {
                    m[i][j] = 2;
                } else {
                    m[i][j] = 4;
                }
                break;
            }
        }
    }
}
/* up se aseamana mult cu celelalte doua functii,
   doar ca citirea se face de sus in jos,
   iar toate casutele se misca in sus*/
void up(int m[][4], int *score, int *moves) {
    int i, j, aux, x, ok = 0, ok1, ok2, ok3, ok4;
    for (j = 0; j < 4; j++) {
        ok1 = 1;
        ok2 = 1;
        ok3 = 1;
        ok4 = 1;
        if(m[2][j] == 0){
            ok3 = 0;
        }
        for (i = 1; i < 4; i++) {
            aux = i;
            while (aux > 0 && m[aux - 1][j] == 0) {
                if (m[aux][j] != 0) {
                    ok = 1;
                }
                m[aux - 1][j] = m[aux][j];
                m[aux][j] = 0;
                aux--;
            }
            if (aux != 0 && m[aux - 1][j] != 0) {
                if (i == 1 && m[aux][j] == m[aux - 1][j]) {
                    if (m[aux][j] != 0) {
                        ok = 1;
                    }
                    m[aux - 1][j] = 2 * m[aux - 1][j];
                    m[aux][j] = 0;
                    *score = *score + m[aux - 1][j];
                    ok1 = 0;
                }
                if (i == 2 && ok1 && m[aux][j] == m[aux - 1][j]) {
                    if (m[aux][j] != 0) {
                        ok = 1;
                    }
                    m[aux - 1][j] = 2 * m[aux - 1][j];
                    m[aux][j] = 0;
                    *score = *score + m[aux - 1][j];
                    ok2 = 0;
                }
                if(ok3 == 0 && ok1 == 0){
                    ok4 = 0;
                }
                if (i == 3 && ok2 && ok4 && m[aux][j] == m[aux - 1][j]) {
                    if (m[aux][j] != 0) {
                        ok = 1;
                    }
                    m[aux - 1][j] = 2 * m[aux - 1][j];
                    m[aux][j] = 0;
                    *score = *score + m[aux - 1][j];
                }
            }
        }
    }
    if (ok) {
        (*moves)++;
        srand(time(NULL));
        while (1) {
            i = rand() % 4;
            j = rand() % 4;
            if (m[i][j] == 0) {
                x = rand() % 2;
                if (x) {
                    m[i][j] = 2;
                } else {
                    m[i][j] = 4;
                }
                break;
            }
        }
    }
}
// analog
void down(int m[][4], int *score, int *moves) {
    int i, j, aux, x, ok = 0, ok1, ok2, ok3, ok4;
    for (j = 0; j < 4; j++) {
        ok1 = 1;
        ok2 = 1;
        ok3 = 1;
        ok4 = 1;
        if(m[1][j] == 0){
            ok3 = 0;
        }
        for (i = 2; i > -1; i--) {
            aux = i;
            while (aux < 3 && m[aux + 1][j] == 0) {
                if (m[aux][j] != 0) {
                    ok = 1;
                }
                m[aux + 1][j] = m[aux][j];
                m[aux][j] = 0;
                aux++;
            }
            if (aux != 3 && m[aux + 1][j] != 0) {
                if (i == 2 && m[aux][j] == m[aux + 1][j]) {
                    if (m[aux][j] != 0) {
                        ok = 1;
                    }
                    m[aux + 1][j] = 2 * m[aux + 1][j];
                    m[aux][j] = 0;
                    *score = *score + m[aux + 1][j];
                    ok1 = 0;
                }
                if (i == 1 && ok1 && m[aux][j] == m[aux + 1][j]) {
                    if (m[aux][j] != 0) {
                        ok = 1;
                    }
                    m[aux + 1][j] = 2 * m[aux + 1][j];
                    m[aux][j] = 0;
                    *score = *score + m[aux + 1][j];
                    ok2 = 0;
                }
                if(ok3 == 0 && ok1 == 0){
                    ok4 = 0;
                }
                if (i == 0 && ok2 && ok4 && m[aux][j] == m[aux + 1][j]) {
                    if (m[aux][j] != 0) {
                        ok = 1;
                    }
                    m[aux + 1][j] = 2 * m[aux + 1][j];
                    m[aux][j] = 0;
                    *score = *score + m[aux + 1][j];
                }
            }
        }
    }
    if (ok) {
        (*moves)++;
        srand(time(NULL));
        while (1) {
            i = rand() % 4;
            j = rand() % 4;
            if (m[i][j] == 0) {
                x = rand() % 2;
                if (x) {
                    m[i][j] = 2;
                } else {
                    m[i][j] = 4;
                }
                break;
            }
        }
    }
}
// functie care determina daca am pierdut sau nu
void lost(int m[][4], int copy_final[][4], int *ok_move_final) {
    int i, j, aux_score = 0, aux_moves = 0;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            copy_final[i][j] = m[i][j];
        }
    }
    right(copy_final, &aux_score, &aux_moves);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            /* daca se gaseste o diferenta intre copia modificata si matricea
               noastra, atunci inseamna ca inca exista cel putin o mutare
               valida*/
            if (copy_final[i][j] != m[i][j]) {
                *ok_move_final = 1;
            }
        }
    }
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            copy_final[i][j] = m[i][j];
        }
    }
    left(copy_final, &aux_score, &aux_moves);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (copy_final[i][j] != m[i][j]) {
                *ok_move_final = 1;
            }
        }
    }
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            copy_final[i][j] = m[i][j];
        }
    }
    up(copy_final, &aux_score, &aux_moves);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (copy_final[i][j] != m[i][j]) {
                *ok_move_final = 1;
            }
        }
    }
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            copy_final[i][j] = m[i][j];
        }
    }
    down(copy_final, &aux_score, &aux_moves);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (copy_final[i][j] != m[i][j]) {
                *ok_move_final = 1;
            }
        }
    }
}
// functie care determina daca am castigat sau nu
void won(int m[][4], int *ok_win) {
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            /* daca se gaseste o celula cu valoarea 2048, 
               inseamna ca am castigat*/
            if (m[i][j] == 2048) {
                *ok_win = 1;
            }
        }
    }
}
int main() {
    int ch, i, j, x, y, k = 0, z, m[4][4] = {0}, score = 0, xmax,
                        ymax, copy[4][4], ok_bomb = 1, best_score = 0,
                        aux_score = 0, ok_move = 0,
                        ok_win = 0, empty = 0, max_empty = -1, ok_move_final,
                        copy_final[4][4], ok_lose = 0, right_moves = 0, left_moves = 0, up_moves = 0, down_moves = 0, aux_moves = 0;
    // variabila pentru afisarea timpului
    time_t t;
    // initiem jocul
    start(m);
    // functie pentru inceput in ncurses
    initscr();
    // pentru a nu afisa ce introducem de la tastatura
    noecho();
    // pentru a nu afisa pozitia cursorului
    curs_set(FALSE);
    // initializare culori
    start_color();
    init_color(8, 999, 400, 0);
    init_color(9, 999, 700, 700);
    init_color(10, 9, 99, 999);
    init_color(11, 500, 300, 0);
    init_color(12, 400, 400, 400);
    init_color(13, 150, 999, 700);
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    init_pair(2, COLOR_CYAN, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_RED, COLOR_BLACK);
    init_pair(5, 10, COLOR_BLACK);
    init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(7, 8, COLOR_BLACK);
    init_pair(8, 9, COLOR_BLACK);
    init_pair(9, 11, COLOR_BLACK);
    init_pair(10, 12, COLOR_BLACK);
    init_pair(11, COLOR_WHITE, COLOR_BLACK);
    init_pair(12, 13, COLOR_BLACK);
    // ferestre pentru castig, pierdere, si cele 16 celule
    WINDOW *win = newwin(9, 70, 12, 69);
    WINDOW *lose = newwin(9, 70, 12, 69);
    WINDOW *grid[4][4];
    // afiseaza in terminal schimbarile facute
    refresh();
    while (1) {
        ok_move_final = 0;
        // diverse afisari
        WINDOW *box_score = newwin(5, 40, 7, 69);
        wmove(box_score, 0, 1);
        wprintw(box_score, "SCORE: %d", score);
        wrefresh(box_score);
        WINDOW *box_best_score = newwin(5, 40, 7, 86);
        wmove(box_best_score, 0, 1);
        wprintw(box_best_score, " BEST SCORE: %d", best_score);
        wrefresh(box_best_score);
        WINDOW *menu = newwin(5, 40, 7, 110);
        wmove(menu, 0, 1);
        wprintw(menu, "PRESS Q FOR MENU");
        wrefresh(menu);
        WINDOW *title = newwin(1, 10, 4, 90);
        wmove(title, 0, 1);
        wprintw(title, "~2048~");
        wrefresh(title);
        // acum construim celulele in terminal
        // fiecare culoare este atribuita unei valori din celula
        for (j = 0; j < 4; j++) {
            for (i = 0; i < 4; i++) {
                y = j * 6 + 2;
                x = i * 13 + k + 5;
                k++;
                grid[i][j] = newwin(6, 13, y, x);
                if (m[j][i] == 0) {
                    wattron(grid[i][j], COLOR_PAIR(5));
                    box(grid[i][j], 0, 0);
                    wattroff(grid[i][j], COLOR_PAIR(5));
                    wrefresh(grid[i][j]);
                }
                if (m[j][i] == 2) {
                    wattron(grid[i][j], COLOR_PAIR(7));
                    box(grid[i][j], 0, 0);
                    wattroff(grid[i][j], COLOR_PAIR(7));
                    wrefresh(grid[i][j]);
                }
                if (m[j][i] == 4) {
                    wattron(grid[i][j], COLOR_PAIR(1));
                    box(grid[i][j], 0, 0);
                    wattroff(grid[i][j], COLOR_PAIR(1));
                    wrefresh(grid[i][j]);
                }
                if (m[j][i] == 8) {
                    wattron(grid[i][j], COLOR_PAIR(2));
                    box(grid[i][j], 0, 0);
                    wattroff(grid[i][j], COLOR_PAIR(2));
                    wrefresh(grid[i][j]);
                }
                if (m[j][i] == 16) {
                    wattron(grid[i][j], COLOR_PAIR(3));
                    box(grid[i][j], 0, 0);
                    wattroff(grid[i][j], COLOR_PAIR(3));
                    wrefresh(grid[i][j]);
                }
                if (m[j][i] == 32) {
                    wattron(grid[i][j], COLOR_PAIR(4));
                    box(grid[i][j], 0, 0);
                    wattroff(grid[i][j], COLOR_PAIR(4));
                    wrefresh(grid[i][j]);
                }
                if (m[j][i] == 64) {
                    wattron(grid[i][j], COLOR_PAIR(8));
                    box(grid[i][j], 0, 0);
                    wattroff(grid[i][j], COLOR_PAIR(8));
                    wrefresh(grid[i][j]);
                }
                if (m[j][i] == 128) {
                    wattron(grid[i][j], COLOR_PAIR(6));
                    box(grid[i][j], 0, 0);
                    wattroff(grid[i][j], COLOR_PAIR(6));
                    wrefresh(grid[i][j]);
                }
                if (m[j][i] == 256) {
                    wattron(grid[i][j], COLOR_PAIR(9));
                    box(grid[i][j], 0, 0);
                    wattroff(grid[i][j], COLOR_PAIR(9));
                    wrefresh(grid[i][j]);
                }
                if (m[j][i] == 512) {
                    wattron(grid[i][j], COLOR_PAIR(10));
                    box(grid[i][j], 0, 0);
                    wattroff(grid[i][j], COLOR_PAIR(10));
                    wrefresh(grid[i][j]);
                }
                if (m[j][i] == 1024) {
                    wattron(grid[i][j], COLOR_PAIR(11));
                    box(grid[i][j], 0, 0);
                    wattroff(grid[i][j], COLOR_PAIR(11));
                    wrefresh(grid[i][j]);
                }
                if (m[j][i] == 2048) {
                    wattron(grid[i][j], COLOR_PAIR(12));
                    box(grid[i][j], 0, 0);
                    wattroff(grid[i][j], COLOR_PAIR(12));
                    wrefresh(grid[i][j]);
                }
            }
            k = 0;
        }
        // afisam si centram valorile din celule si le coloram corespunzator
        for (j = 0; j < 4; j++) {
            for (i = 0; i < 4; i++) {
                if (m[j][i] != 0) {
                    // aflam dimensiunea celulelor pentru centrare
                    getmaxyx(grid[i][j], ymax, xmax);
                    if (m[j][i] == 2) {
                        wmove(grid[i][j], ymax / 2 - 1, xmax / 2);
                        wattron(grid[i][j], COLOR_PAIR(7));
                        wprintw(grid[i][j], "%d", m[j][i]);
                        wattroff(grid[i][j], COLOR_PAIR(7));
                        wrefresh(grid[i][j]);
                    }
                    if (m[j][i] == 4) {
                        wmove(grid[i][j], ymax / 2 - 1, xmax / 2);
                        wattron(grid[i][j], COLOR_PAIR(1));
                        wprintw(grid[i][j], "%d", m[j][i]);
                        wattroff(grid[i][j], COLOR_PAIR(1));
                        wrefresh(grid[i][j]);
                    }
                    if (m[j][i] == 8) {
                        wmove(grid[i][j], ymax / 2 - 1, xmax / 2);
                        wattron(grid[i][j], COLOR_PAIR(2));
                        wprintw(grid[i][j], "%d", m[j][i]);
                        wattroff(grid[i][j], COLOR_PAIR(2));
                        wrefresh(grid[i][j]);
                    }
                    if (m[j][i] == 16) {
                        wmove(grid[i][j], ymax / 2 - 1, xmax / 2);
                        wattron(grid[i][j], COLOR_PAIR(3));
                        wprintw(grid[i][j], "%d", m[j][i]);
                        wattroff(grid[i][j], COLOR_PAIR(3));
                        wrefresh(grid[i][j]);
                    }
                    if (m[j][i] == 32) {
                        wmove(grid[i][j], ymax / 2 - 1, xmax / 2);
                        wattron(grid[i][j], COLOR_PAIR(4));
                        wprintw(grid[i][j], "%d", m[j][i]);
                        wattroff(grid[i][j], COLOR_PAIR(4));
                        wrefresh(grid[i][j]);
                    }
                    if (m[j][i] == 64) {
                        wmove(grid[i][j], ymax / 2 - 1, xmax / 2);
                        wattron(grid[i][j], COLOR_PAIR(8));
                        wprintw(grid[i][j], "%d", m[j][i]);
                        wattroff(grid[i][j], COLOR_PAIR(8));
                        wrefresh(grid[i][j]);
                    }
                    if (m[j][i] == 128) {
                        wmove(grid[i][j], ymax / 2 - 1, xmax / 2 - 1);
                        wattron(grid[i][j], COLOR_PAIR(6));
                        wprintw(grid[i][j], "%d", m[j][i]);
                        wattroff(grid[i][j], COLOR_PAIR(6));
                        wrefresh(grid[i][j]);
                    }
                    if (m[j][i] == 256) {
                        wmove(grid[i][j], ymax / 2 - 1, xmax / 2 - 1);
                        wattron(grid[i][j], COLOR_PAIR(9));
                        wprintw(grid[i][j], "%d", m[j][i]);
                        wattron(grid[i][j], COLOR_PAIR(9));
                        wrefresh(grid[i][j]);
                    }
                    if (m[j][i] == 512) {
                        wmove(grid[i][j], ymax / 2 - 1, xmax / 2 - 1);
                        wattron(grid[i][j], COLOR_PAIR(10));
                        wprintw(grid[i][j], "%d", m[j][i]);
                        wattroff(grid[i][j], COLOR_PAIR(10));
                        wrefresh(grid[i][j]);
                    }
                    if (m[j][i] == 1024) {
                        wmove(grid[i][j], ymax / 2 - 1, xmax / 2 - 1);
                        wattron(grid[i][j], COLOR_PAIR(11));
                        wprintw(grid[i][j], "%d", m[j][i]);
                        wattroff(grid[i][j], COLOR_PAIR(11));
                        wrefresh(grid[i][j]);
                    }
                    if (m[j][i] == 2048) {
                        wmove(grid[i][j], ymax / 2 - 1, xmax / 2 - 1);
                        wattron(grid[i][j], COLOR_PAIR(12));
                        wprintw(grid[i][j], "%d", m[j][i]);
                        wattroff(grid[i][j], COLOR_PAIR(12));
                        wrefresh(grid[i][j]);
                    }
                }
            }
        }
        // daca s-a castigat se afiseaza un mesaj
        if (ok_win == 1) {
            wmove(win, 1, 0);
            wprintw(win, "YOU WON! CONGRATULATIONS!");
            //acestea sunt niste statistici
            wmove(win, 4, 20);
            //cate mutari au fost facute la dreapta etc
            wprintw(win,"Right moves made: %d", right_moves);
            wmove(win, 4, 45);
            wprintw(win,"Left moves made: %d", left_moves);
            wmove(win, 6, 20);
            wprintw(win,"Up moves made: %d", up_moves);
            wmove(win, 6, 45);
            wprintw(win,"Down moves made: %d", down_moves);
            wmove(win, 8, 20);
            if(ok_bomb){
                 wprintw(win, "You did not use the bomb in this game!");
            }else{
                wprintw(win, "You used the bomb in this game!");
            }
            wrefresh(win);
            keypad(win, TRUE);
            i = 0;
            right_moves = 0;
            left_moves = 0;
            up_moves = 0;
            down_moves = 0;
            /* aici am construit practic un meniu care
               ne permite sa sa alegem intre un nou joc
               si iesirea din aplicatie*/
            // cat timp tasta enter nu este apasata
            while (ch != '\n') {
                // pentru a nu iesi din meniu
                if (i == -1) {
                    i = 0;
                }
                // pentru a nu iesi din meniu
                if (i == 2) {
                    i = 1;
                }
                /* daca ne aflam pe prima varianta, aceasta
                   va primi un efect care ne indica acest aspect*/
                if (i == 0) {
                    wattron(win, A_STANDOUT);
                    mvwprintw(win, 4, 0, "NEW GAME");
                    wattroff(win, A_STANDOUT);
                    mvwprintw(win, 5, 0, "QUIT");
                    wrefresh(win);
                    ch = wgetch(win);
                }
                // la fel si pentru varianta de iesire
                if (i == 1) {
                    mvwprintw(win, 4, 0, "NEW GAME");
                    wattron(win, A_STANDOUT);
                    mvwprintw(win, 5, 0, "QUIT");
                    wattroff(win, A_STANDOUT);
                    wrefresh(win);
                    ch = wgetch(win);
                }
                // meniul se deruleaza in jos sau in sus
                if (ch == KEY_DOWN) {
                    i++;
                }
                if (ch == KEY_UP) {
                    i--;
                }
            }
            /* daca varianta de joc nou a fost accesata, atunci
               scorul se va compara cu scorul maxim, iar apoi va
               ajunge sa fie din nou 0*/
            if (i == 0) {
                if (best_score < score) {
                    best_score = score;
                }
                score = 0;
                ok_bomb = 1;
                // matricea se reseteaza
                matrix(m);
                // se atribuie cele 2 valori de inceput
                start(m);
                werase(win);
                // se afiseaza modificarile
                wrefresh(win);
            }
            // daca se alege varianta a doua, se iese din joc
            if (i == 1) {
                break;
            }
        }
        // asemanator cu situatia in care am castigat
        if (ok_lose == 1) {
            wmove(lose, 0, 0);
            wprintw(lose, "YOU LOST! CONGRATULATIONS! TRY AGAIN?");
            //statistici
            wmove(lose, 4, 20);
            wprintw(lose,"Right moves made: %d", right_moves);
            wmove(lose, 4, 45);
            wprintw(lose,"Left moves made: %d", left_moves);
            wmove(lose, 6, 20);
            wprintw(lose,"Up moves made: %d", up_moves);
            wmove(lose, 6, 45);
            wprintw(lose,"Down moves made: %d", down_moves);
            wmove(lose, 8, 20);
            if(ok_bomb){
                 wprintw(lose, "You did not use the bomb in this game!");
            }else{
                wprintw(lose, "You used the bomb in this game!");
            }
            wrefresh(lose);
            keypad(lose, TRUE);
            i = 0;
            right_moves = 0;
            left_moves = 0;
            up_moves = 0;
            down_moves = 0;
            while (ch != '\n') {
                if (i == -1) {
                    i = 0;
                }
                if (i == 2) {
                    i = 1;
                }
                if (i == 0) {
                    wattron(lose, A_STANDOUT);
                    mvwprintw(lose, 4, 0, "NEW GAME");
                    wattroff(lose, A_STANDOUT);
                    mvwprintw(lose, 5, 0, "QUIT");
                    wrefresh(lose);
                    ch = wgetch(lose);
                }
                if (i == 1) {
                    mvwprintw(lose, 4, 0, "NEW GAME");
                    wattron(lose, A_STANDOUT);
                    mvwprintw(lose, 5, 0, "QUIT");
                    wattroff(lose, A_STANDOUT);
                    wrefresh(lose);
                    ch = wgetch(lose);
                }
                if (ch == KEY_DOWN) {
                    i++;
                }
                if (ch == KEY_UP) {
                    i--;
                }
            }
            if (i == 0) {
                if (best_score < score) {
                    best_score = score;
                }
                score = 0;
                ok_bomb = 1;
                matrix(m);
                start(m);
                werase(lose);
                wrefresh(lose);
            }
            if (i == 1) {
                break;
            }
        }
        // functie care permite inregistrarea sagetilor
        keypad(stdscr, TRUE);
        /* practic aceasta functie ii permite functiei
           getch() sa nu mai astepte neaparat input*/
        nodelay(stdscr, TRUE);
        refresh();
        // daca nu s-a castigat sau nu s-a pierdut
        if (ok_win == 0 && ok_lose == 0) {
            // cat timp nu s-a introdus nimic de la tastatura
            while ((ch = getch()) == ERR) {
                // afisam timpul actual, care isi da refresh la 2 tick-uri
                time(&t);
                WINDOW *time = newwin(1, 30, 24, 69);
                wmove(time, 0, 1);
                wprintw(time, "%s", ctime(&t));
                wrefresh(time);
                // daca au trecut 10000 de tick-uri(10 secunde)
                if (k > 10000) {
                    // cream o copie la matrice
                    for (i = 0; i < 4; i++) {
                        for (j = 0; j < 4; j++) {
                            copy[i][j] = m[i][j];
                        }
                    }
                    /* practic acum vom verifica fiecare mutare in parte,
                       pentru a determina care mutare ne va oferi cele
                       mai multe celule goale */
                    /* aux_score nu ne ajuta cu nimic, dar a trebuit
                       sa creez o variabila pe care sa o bag in functie :))*/
                    right(copy, &aux_score, &aux_moves);
                    for (i = 0; i < 4; i++) {
                        for (j = 0; j < 4; j++) {
                            // verificam daca mutarea este posibila
                            if (copy[i][j] != m[i][j]) {
                                ok_move = 1;
                            }
                            /* numaram cate celule ajung sa fie goale dupa
                               aplicarea functiei*/
                            if (copy[i][j] == 0) {
                                empty++;
                            }
                        }
                    }
                    // se cauta cea mai mare valoare a casutelor goale
                    if (empty > max_empty && ok_move) {
                        max_empty = empty;
                        z = 0;
                    }
                    empty = 0;
                    ok_move = 0;
                    // si repetam procesul si pentru celelalte functii
                    for (i = 0; i < 4; i++) {
                        for (j = 0; j < 4; j++) {
                            copy[i][j] = m[i][j];
                        }
                    }
                    left(copy, &aux_score, &aux_moves);
                    for (i = 0; i < 4; i++) {
                        for (j = 0; j < 4; j++) {
                            if (copy[i][j] != m[i][j]) {
                                ok_move = 1;
                            }
                            if (copy[i][j] == 0) {
                                empty++;
                            }
                        }
                    }
                    if (empty > max_empty && ok_move) {
                        max_empty = empty;
                        z = 1;
                    }
                    empty = 0;
                    ok_move = 0;
                    for (i = 0; i < 4; i++) {
                        for (j = 0; j < 4; j++) {
                            copy[i][j] = m[i][j];
                        }
                    }
                    up(copy, &aux_score, &aux_moves);
                    for (i = 0; i < 4; i++) {
                        for (j = 0; j < 4; j++) {
                            if (copy[i][j] != m[i][j]) {
                                ok_move = 1;
                            }
                            if (copy[i][j] == 0) {
                                empty++;
                            }
                        }
                    }
                    if (empty > max_empty && ok_move) {
                        max_empty = empty;
                        z = 2;
                    }
                    empty = 0;
                    ok_move = 0;
                    for (i = 0; i < 4; i++) {
                        for (j = 0; j < 4; j++) {
                            copy[i][j] = m[i][j];
                        }
                    }
                    down(copy, &aux_score, &aux_moves);
                    for (i = 0; i < 4; i++) {
                        for (j = 0; j < 4; j++) {
                            if (copy[i][j] != m[i][j]) {
                                ok_move = 1;
                            }
                            if (copy[i][j] == 0) {
                                empty++;
                            }
                        }
                    }
                    if (empty > max_empty && ok_move) {
                        max_empty = empty;
                        z = 3;
                    }
                    empty = 0;
                    ok_move = 0;
                    max_empty = -1;
                    /* am facut rost de functia care ne ofera
                       cel mai mare numar de celule goale*/
                    if (z == 0) {
                        ch = KEY_RIGHT;
                    }
                    if (z == 1) {
                        ch = KEY_LEFT;
                    }
                    if (z == 2) {
                        ch = KEY_UP;
                    }
                    if (z == 3) {
                        ch = KEY_DOWN;
                    }
                    break;
                }
                napms(2);
                k++;
            }
            k = 0;
            nodelay(stdscr, FALSE);
            /* apelam functia pe care ne-am ales-o sau pe care
               a ales-o "inteligenta artificiala"*/
            if (ch == KEY_RIGHT) {
                right(m, &score, &right_moves);
            }
            if (ch == KEY_LEFT) {
                left(m, &score, &left_moves);
            }
            if (ch == KEY_UP) {
                up(m, &score, &up_moves);
            }
            if (ch == KEY_DOWN) {
                down(m, &score, &down_moves);
            }
        }
        ok_win = 0;
        /* daca se gaseste un element in matrice care
           are valoarea 2048 se activeaza variabila de ok_win,
           folosita mai sus*/
        won(m, &ok_win);
        ok_lose = 0;
        /* aceasta bucata de cod verifica daca exista
           mutari legale, iar daca nu exista
           se activeaza variabila ok_lose, folosita mai sus*/
        lost(m, copy_final, &ok_move_final);
        if (ok_move_final == 0) {
            ok_lose = 1;
        }
        /* daca tasta Q a fost apasata se deschide un meniu prin
           care ne putem misca, asemanator cu cele precedente*/
        // in plus mai avem optiunile de resume si rules
        if (ch == 'q') {
            WINDOW *options = newwin(7, 30, 12, 70);
            box(options, 0, 0);
            i = 0;
            keypad(options, TRUE);
            // cat timp nu se introduce de la tastatura enter
            while (ch != '\n') {
                // pentru a nu iesi din meniu
                if (i == -1) {
                    i = 0;
                }
                if (i == 4) {
                    i = 3;
                }
                /* se marcheaza corespunzator optiunea la care 
                   ne aflam cu un highlight*/ 
                if (i == 0) {
                    wattron(options, A_STANDOUT);
                    mvwprintw(options, 1, 0, "NEW GAME");
                    wattroff(options, A_STANDOUT);
                    mvwprintw(options, 2, 0, "RESUME");
                    mvwprintw(options, 3, 0, "QUIT");
                    mvwprintw(options, 4, 0, "RULES");
                    wrefresh(options);
                    ch = wgetch(options);
                }
                if (i == 1) {
                    mvwprintw(options, 1, 0, "NEW GAME");
                    wattron(options, A_STANDOUT);
                    mvwprintw(options, 2, 0, "RESUME");
                    wattroff(options, A_STANDOUT);
                    mvwprintw(options, 3, 0, "QUIT");
                    mvwprintw(options, 4, 0, "RULES");
                    wrefresh(options);
                    ch = wgetch(options);
                }
                if (i == 2) {
                    mvwprintw(options, 1, 0, "NEW GAME");
                    mvwprintw(options, 2, 0, "RESUME");
                    wattron(options, A_STANDOUT);
                    mvwprintw(options, 3, 0, "QUIT");
                    wattroff(options, A_STANDOUT);
                    mvwprintw(options, 4, 0, "RULES");
                    wrefresh(options);
                    ch = wgetch(options);
                }
                if (i == 3) {
                    mvwprintw(options, 1, 0, "NEW GAME");
                    mvwprintw(options, 2, 0, "RESUME");
                    mvwprintw(options, 3, 0, "QUIT");
                    wattron(options, A_STANDOUT);
                    mvwprintw(options, 4, 0, "RULES");
                    wattroff(options, A_STANDOUT);
                    wrefresh(options);
                    ch = wgetch(options);
                }
                if (ch == KEY_DOWN) {
                    i++;
                }
                if (ch == KEY_UP) {
                    i--;
                }
            }
            // optiunea resume ne intoarce la joc
            if (i == 1) {
                werase(options);
                wrefresh(options);
            }
            // optiunea quit ne scoate din joc
            if (i == 2) {
                break;
            }
            // optiunea new game reinitializeaza jocul
            if (i == 0) {
                if (best_score < score) {
                    best_score = score;
                }
                score = 0;
                ok_bomb = 1;
                matrix(m);
                start(m);
                werase(options);
                wrefresh(options);
            }
            // optiunea rules ne deschide o fereastra cu regulile jocului
            if (i == 3) {
                werase(options);
                wrefresh(options);
                WINDOW *rules = newwin(7, 70, 12, 70);
                do {
                    wmove(rules, 1, 0);
                    wprintw(rules, "The movement is done by arrows");
                    wmove(rules, 2, 0);
                    wprintw(rules, "Press B to activate the bomb");
                    wmove(rules, 3, 0);
                    wprintw(rules, "The bomb destroys every 2 and 4 cell");
                    wmove(rules, 4, 0);
                    wprintw(
                        rules,
                        "It can be activated after 2000 points once per game");
                    wmove(rules, 5, 0);
                    wprintw(rules,
                            "It can not be activated when there are no legal "
                            "moves");
                    wmove(rules, 6, 0);
                    wprintw(rules, "Press ENTER to exit");
                    wrefresh(rules);
                } while ((ch = wgetch(rules)) != '\n');
                werase(rules);
                wrefresh(rules);
            }
        }
        /* daca tasta B este apasata si scorul a depasit 2000 de puncte
           se va activa o bomba care elimina toate celulele cu valorile 2 sau 4,
           dar aceasta se poate ultiliza o singura data per joc*/
        if (ch == 'b' && ok_bomb && score >= 100) {
            for (i = 0; i < 4; i++) {
                for (j = 0; j < 4; j++) {
                    if (m[i][j] == 2 || m[i][j] == 4) {
                        m[i][j] = 0;
                    }
                }
            }
            ok_bomb = 0;
        }
    }
    endwin();
    return 0;
}
