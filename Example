# Ещё раз про Правило 3/5/0

Есть ещё одно правило, называется "никому нельзя верить".
Вот мы такие молодцы решили поискать дополнительный материал по теме, гуглим и попадаем на сайт [cppreference.com](https://en.cppreference.com/w/cpp/language/rule_of_three). Смотрим примеры и... что за? Я ничего не поняла. "Эти правила слишком сложные, я не хочу с этим иметь дело" - вполне адекватная реакция на это.
Я приведу примеры здесь для наглядности.

### Правило трех.

```cpp
class rule_of_three
{
    char* cstring; // raw pointer used as a handle to a dynamically-allocated memory block
    rule_of_three(const char* s, std::size_t n) // to avoid counting twice
    : cstring(new char[n]) // allocate
    {
        std::memcpy(cstring, s, n); // populate
    }
 public:
    rule_of_three(const char* s = "")
    : rule_of_three(s, std::strlen(s) + 1)
    {}
    ~rule_of_three()
    {
        delete[] cstring;  // deallocate
    }
    rule_of_three(const rule_of_three& other) // copy constructor
    : rule_of_three(other.cstring)
    {}
    rule_of_three& operator=(rule_of_three other) // copy assignment
    {
        std::swap(cstring, other.cstring);
        return *this;
    }
};
```

### Правило пяти

```cpp
class rule_of_five
{
    char* cstring; // raw pointer used as a handle to a dynamically-allocated memory block
    rule_of_five(const char* s, std::size_t n) // to avoid counting twice
    : cstring(new char[n]) // allocate
    {
        std::memcpy(cstring, s, n); // populate
    }
 public:
    rule_of_five(const char* s = "")
    : rule_of_five(s, std::strlen(s) + 1)
    {}
    ~rule_of_five()
    {
        delete[] cstring;  // deallocate
    }
    rule_of_five(const rule_of_five& other) // copy constructor
    : rule_of_five(other.cstring)
    {}
    rule_of_five(rule_of_five&& other) noexcept // move constructor
    : cstring(std::exchange(other.cstring, nullptr))
    {}
    rule_of_five& operator=(const rule_of_five& other) // copy assignment
    {
         return *this = rule_of_five(other);
    }
    rule_of_five& operator=(rule_of_five&& other) noexcept // move assignment
    {
        std::swap(cstring, other.cstring);
        return *this;
    }
// alternatively, replace both assignment operators with 
//  rule_of_five& operator=(rule_of_five other) noexcept
//  {
//      std::swap(cstring, other.cstring);
//      return *this;
//  }
};
```

### Правило ноль
Ну ок, тут у меня нет вопросов, все понятно.
```cpp
class rule_of_zero
{
    std::string cppstring;
 public:
    rule_of_zero(const std::string& arg) : cppstring(arg) {}
};
```

Займемся поиском ошибок и исправлений. Продолжение на занятие))
