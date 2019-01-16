#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>

// правило трех
class StringRule3
{
public:
    //конструктор
    StringRule3(const char *s = ""): // использование значения по умолчанию
    StringRule3(s, std::strlen(s) + 1) //вызов делигирующего конструктора
    {
        std::cout << "rule 3 ctor" << std::endl;
    }

    //конструктор копирования
    StringRule3(const StringRule3 &rhs) :
    StringRule3(rhs.c_str)
    {
        std::cout << "rule 3 copy ctor" << std::endl;
    }

    //оператор присваивания
    StringRule3& operator=(const StringRule3 &rhs)
    {
        std::cout << "rule 3 copy operator=" << std::endl;

        delete[]c_str;
        c_str = new char[strlen(rhs.c_str)];
        strcpy(c_str, rhs.c_str);

        return *this;
    }

    StringRule3(StringRule3&& other) = delete;
    StringRule3& operator=(StringRule3&& other) = delete;

    friend std::ostream &operator<<(std::ostream &os, StringRule3 const &str);

    ~StringRule3()
    {
        std::cout << "rule 3 dtor" << std::endl;
        delete[] c_str;
    }


private:
    char *c_str;

    StringRule3(const char * s, std::size_t n):
            c_str(new char[n])
    {
        std::memcpy(c_str, s, n);
    }

};

std::ostream &operator<<(std::ostream &os, StringRule3 const &str)
{
    std::cout << str.c_str << "\n";
}


class StringRule5
{
public:
    StringRule5(const char *s = ""):
            c_str(new char[std::strlen(s)])
    {
        strcpy(c_str, s);
        std::cout << "rule 5 ctor" << std::endl;
    }

    //конструктор копирования
    StringRule5(const StringRule5 &rhs):
            c_str(new char[std::strlen(rhs.c_str)])
    {
        strcpy(c_str, rhs.c_str);
        std::cout << "rule 5 copy ctor" << std::endl;
    }

    //оператор присваивания
    StringRule5& operator=(const StringRule5 & rhs)
    {
        std::cout << "rule 5 copy operator=" << std::endl;

        delete[]c_str;
        c_str = new char[strlen(rhs.c_str)];
        strcpy(c_str, rhs.c_str);

        return *this;

    }

    //конструктор перемещения
    StringRule5(StringRule5 && rhs)
    {
        std::cout << "rule 5 move ctor" << std::endl;

        c_str = rhs.c_str;
        rhs.c_str = nullptr;
    }

    //оператор перемещения
    StringRule5& operator=(StringRule5 &&rhs)
    {
        std::cout << "rule 5 move operator=" << std::endl;
        std::swap(c_str, rhs.c_str);
        return *this;
    }

    ~StringRule5()
    {
        std::cout << "rule 5 dtor" << std::endl;
        delete []c_str;
    }


    friend std::ostream &operator<<(std::ostream &os, StringRule5 const &str);



private:
    char *c_str;

};

std::ostream &operator<<(std::ostream &os, StringRule5 const &str)
{
    std::cout << str.c_str << "\n";
}

class StringRule0
{
public:
    StringRule0(const std::string& str):
    str(str)
    {}

private:
    std::string str;

};

int main()
{
    StringRule3 rule_three_1("Hello");
    StringRule3 rule_three_2("!");

    StringRule3 rule_three_3 = rule_three_1;
    std::cout << rule_three_2 << rule_three_3;

    rule_three_2 = rule_three_3;

    std::cout << rule_three_1 << rule_three_2 << rule_three_3;


    StringRule5 rule_five_1("Hello");
    StringRule5 rule_five_2("!");

    StringRule5 rule_five_3(rule_five_1);

    std::cout << rule_five_2 << rule_five_3;

    rule_five_2 = rule_five_3;

    std::cout << rule_five_1 << rule_five_2 << rule_five_3;

    rule_five_1 = std::move(rule_five_2);

    StringRule5 rule_five_4(std::move(StringRule5("four!")));

    std::cout << rule_five_1 << rule_five_2 << rule_five_3 << rule_five_4;




}
