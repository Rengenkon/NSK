#include <iostream>
#include<fstream>
#include<string.h>
#include <sstream>


class zap {
protected:
    std::string name;
    std::string locate;
    int cost;

public:
    zap(std::string* n = NULL, std::string* l = NULL, int c = NULL) {
        name = *n;
        locate = *l;
        cost = c;
    }

    std::string getname() {
        return name;
    }

    std::string getloc() {
        return locate;
    }

    int getcost() {
        return cost;
    }
};


class point : public zap {
private:
    //zap value;
    point* pred = NULL;
    point* next = NULL;


public:
    point(std::string* n, std::string* l, int c) : zap(n, l, c) {}


    void setp(point* a) {
        pred = a;
    }
    
    void setn(point* a) {
        next = a;
    }

    point* getp() {
        return pred;
    }

    point* getn() {
        return next;
    }
};


class chain {
protected:
    point* focus = NULL;

    void to_first() {//перемещение в начало
        while (focus->getp() != NULL) {
            focus = focus->getp();
        }
    }

    void to_end() {//перемещение в конец
        while (focus->getn() != NULL) {
            focus = focus->getn();
        }
    }

public:
    chain() {

    }

    ~chain() {
        while (focus != NULL)
        {
            del();
        }
    }

    void add(std::string* name, std::string* loc, int cost, int* ind = NULL) {
        if (focus != NULL) {
            point* x = new point(name, loc, cost);

            if (ind == NULL) {
                    to_end();
                    focus->setn(x);
                    x->setp(focus);
            }
            else {
                to_first();
                for (int i = 0; i < *ind; i++) {
                    if (focus->getn() != NULL) {
                        focus = focus->getn();
                    }
                    else {
                        focus->setn(x);
                        x->setp(focus);

                        return;
                    }
                }

                x->setn(focus);
                x->setp(focus->getp());
                focus->getp()->setn(x);
                focus->setp(x);
            }
        }
        else {
            focus = new point(name, loc, cost);
        }
    }

    void del(int* ind = NULL) {
        if (focus != NULL) {
            if (ind != NULL) {
                to_first();
                for (int i = 0; i < *ind; i++) {
                    if (focus->getn() != NULL) {
                        focus = focus->getn();
                    }
                    else {
                        break;
                    }
                }
            }

            if (focus->getp() != NULL) {
                focus->getp()->setn(focus->getn());
            }
            if (focus->getn() != NULL) {
                focus->getn()->setp(focus->getp());
            }

            point* x = focus;

            if (focus->getn() != NULL) {
                focus = focus->getn();
            }
            else if (focus->getp() != NULL) {
                focus = focus->getp();
            }
            else {
                focus = NULL;
            }

            delete x;
        }
        else {
            std::cout << "List is empty";
        }
    }

    




    
    void fiter_zona(std::string z) {
        int count = 0;


        to_first();
        while (focus->getn() != NULL) {
            if (focus->getloc() == z) {
                count++;
            }
            focus = focus->getn();
        }
        if (focus->getloc() == z) {
            count++;
        }
    }
    
    void filter_cost(std::string n, point* line) {

    }

};

class table {
private: 
    int size;
    int column;
    int alignment;

public:
    table(int s = 10, int c = 4) {
        size = s;
        column = c;

        alignment = 1;
        /*
        Выравнивание
        1 - слева
        2 - центр
        3 - справа
        */
    }

    std::string separator() { //+--------+--------+
        std::string s = "";
        for (int i = 0; i < column * (1 + size) + 1; i++) {
            if (i % (size + 1) == 0) {
                s += "+";
            }
            else {
                s += "-";
            }
        }
        s += "\n";
        return s;
    }

    std::string alig(std::string* s1) {
        std::string s2 = "";
        if (alignment == 1) {
            s2 = *s1;
            for (int i = 0; i < size - s1->length(); i++) {
                s2 += " ";
            }
        }
        else if (alignment == 2) {
            for (int i = 0; i < (size - s1->length()) / 2; i++) {
                s2 += " ";
            }
            s2 += *s1;
            for (int i = 0; i < size - s1->length() - (size - s1->length()) / 2; i++) {
                s2 += " ";
            }
        }
        else {
            for (int i = 0; i < size - s1->length(); i++) {
                s2 += " ";
            }
            s2 += *s1;
        }
        return s2;
    }

    std::string line(std::string* s1) {
        std::string s2 ="|";
        for (int i = 0; i < column; i++) {
            s2 += alig(s1 + i);
            s2 += "|";
        }
        s2 += "\n";
        return s2;
    }

};


class menu : public chain{
private:
    bool console = true;
    int stolb;
    int ID;

    std::string source = "";
    std::ifstream* IN;
    std::ofstream* OUT;

    table* t = NULL;

    void (menu::*Function_in_cycle) () = NULL;


    void (menu::* Direction) () = NULL;
    void forward() { // прямой
        if (focus != NULL) {
            to_first();
            while (focus->getn() != NULL) {
                (this->*Function_in_cycle)();
                focus = focus->getn();
            }
            (this->*Function_in_cycle)();
        }
    }
    void reverse() { // обратный
        if (focus != NULL) {
            to_end();
            while (focus->getp() != NULL) {
                (this->*Function_in_cycle)();
                focus = focus->getp();
            }
            (this->*Function_in_cycle)();
        }
    }

    void write() {
        stolb = 4;
        t = new table(10, stolb);

        ID = 1;
        if (console) {
            std::cout << t->separator();
            this->Function_in_cycle = &menu::cons;
        }
        else {
            *OUT << t->separator();
            this->Function_in_cycle = &menu::file;
        }
        (this->*Direction)();
        delete t;
    }

    void cons() {
        std::string* a = new std::string[stolb];

        a[0] = std::to_string(ID);
        a[1] = focus->getname();
        a[2] = focus->getloc();
        a[3] = std::to_string(focus->getcost());

        std::cout << t->line(a);
        std::cout << t->separator();

        delete [] a;
        ID++;
    }

    void file() {
        std::string* a = new std::string[stolb];


        a[0] = std::to_string(ID);
        a[1] = focus->getname();
        a[2] = focus->getloc();
        a[3] = std::to_string(focus->getcost());

        *OUT << t->line(a);
        *OUT << t->separator();

        delete [] a;
        ID++;
    }

    

public:
    menu() {

    }
    ~menu() {
        delete IN;
        delete OUT;
    }

    void read_data() {
        std::string s;
        while (!IN->eof()) {
            std::getline(*IN, s);
            /*
            id?????????????????????для тестов
            имя
            локация
            цена
            */
            std::string data[4] = { "" };

            int count = 0;
            int start = -1, end = -1;
            for (int i = 0; i < s.length(); i++) {
                int ghfs = 1;
                if (s[i] != ' ' and s[i] != '|') {
                    if (start == -1) {
                        start = i;
                    }
                    if (i == s.length() - 1) {
                        end = s.length() - 1;
                    }
                }
                else {
                    if (start != -1) {
                        end = i - 1;
                    }
                }

                if (start != -1 and end != -1) {
                    for (int j = start; j <= end; j++) {
                        data[count] += s[j];
                    }
                    start = -1;
                    end = -1;
                    count++;
                }
            }

            add(data + 1, data + 2, std::stoi(data[3]));
        }

        int tas = 1;
        IN->close();
        OUT = new std::ofstream(source);
    }

    void Fwrite() {
        this->Direction = &menu::forward;
        write();
    }
    
    void Rwrite() {
        this->Direction = &menu::reverse;
        write();
    }

    void main() {
        std::cout << "Введите путь к файлу\n";
        /*std::cin >> source;
        if (source == "") {
            source = "G:\\Ex.txt";
        }
        */source = "G:\\Ex.txt";
        IN = new std::ifstream(source);
        read_data();

        ////////////////////////////////////////////////////////////
        /*
            Показать решение первой задачи
            Показать решение второй задачи
            Отобразить список прямо
            Отобразить список обратно
            Сохранить список прямо
            Сохранить список обратно
        */
        select();

        ////////////////////////////////////////////////////////////

        OUT->close();
    }


    void select() {
        int a;
        table sep(20, 1);
        sep.separator();
        std::cout << sep.separator() <<
            "1 - Показать решение первой задачи\n2 -Показать решение второй задачи\n3 - Отобразить список прямо\n4 - Отобразить список обратно\n5 - Сохранить список прямо\n6 - Сохранить список обратно\n"
            << sep.separator();
        //std::cin >> a;
        a = 6;
        switch (a)
        {
        case 1:

            select();
            break;
        case 2:

            select();
            break;
        case 3:
            console = true;
            Fwrite();
            select();
            break;
        case 4:
            console = true;
            Rwrite();
            select();
            break;
        case 5:
            console = false;
            Fwrite();
            break;
        case 6:
            console = false;
            Rwrite();
            break;
        default:
            std::cout << "НЕВЕРНЫЙ ВВОД";
            select();
            break;
        }
    }
};


int main()
{
    std::setlocale(LC_ALL, "Russian");
    menu A;
    
    A.main();
}