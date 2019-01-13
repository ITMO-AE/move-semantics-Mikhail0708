#include <iostream>
#include <utility>
#include <vector>
#include <string>

int main()
{


    std::string str = "Hello";
    std::vector<std::string> v;

    // использование копирования
    v.push_back(str);
    std::cout << "After copy, str is " << str << "\n";

    // использование перемещения
    v.push_back(std::move(str));
    std::cout << "After move, str is " << str << "\n";

    std::cout << "The contents of the vector are " << v[0]
              << ", " << v[1] << "\n";
}