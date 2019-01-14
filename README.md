## Правила 0/3/5.
Содержание:
[Конструктор копирования](#конструктор-копирования)
[Оператор присваивания](#оператор-присваивания)
[Правило трех](#правило-трех)
[Конструктор перемещения и оператор перемещения](#конструктор-перемещения-и-оператор-перемещения)
[Правило пяти](#правило-пяти)
[Правило нуля](#правило-нуля)

### Конструктор копирования
Создадим класс, который не умеет ничего кроме вывода сообщений о своем создании и уничтожении

```cpp
#include <iostream>
class Example
{
public:
    Example()
    {
        std::cout << "ctor" << std::endl;
    }
    ~Example()
    {
        std::cout << "dtor" << std::endl;
    }
};

```

Создадим два экземпляра этого класса и посмотрим что получится.
```cpp
int main()
{
    Example c1;
    Example c2 = c1;
    return 0;
}
```

Сколько раз выведется "ctor", а сколько раз "dtor"?

### Ожидание:
```sh
ctor
ctor
dtor
dtor
```
### Реальность:
```sh
ctor
dtor
dtor
```
Вопрос дня, а куда делался вызов еще одного конструктора? 
Рассмотрим два примера: <br>
1.
```cpp
int main()
{
    Example c1;
    Example c2;
    c2 = c1;
    return 0;
}
```
2.
```cpp
int main()
{
    Example c1;
    Example c2 = c1;
    return 0;
}

```

В первом случае все ожидаемо, два вызова конструктора, два вызова деструктора.
Во втором случае конструктор вызывается только для создания объекта c1.
Для создания объекта c2 используется конструктор копирования. Пруф:

```cpp
class Example
{
public:
    Example()
    {
        std::cout << "ctor" << std::endl;
    }
    //конструктор копирования
    Example(const Example &)
    {
        std::cout << "copy ctor" << std::endl;
    }
    ~Example()
    {
        std::cout << "dtor" << std::endl;
    }
};

int main()
{
    Example c1;
    Example c2 = c1;
    return 0;
}
```
Запустим и посмотрим вывод:
```sh
ctor
copy ctor
dtor
dtor
```

Для создания объекта `c2` вызывался не обычный конструктор, а конструктор копирования (конструктор копий) по умолчанию, который за нас сгенерировал компилятор. После того как конструктор копирования был определен явно, мы смогли убедиться что именно он стоит за созданием объекта `c2`.

#### Небольшой пример иллюстрирующий необходимость явной реализации конструктора копирования

```cpp
#include <iostream>
#include <string.h>

class Example
{
public:
    Example(const char *value)
    {
        ptr = new char[strlen(value)];
        strcpy(ptr, value);
        std::cout << "ctor" << std::endl;
    }
    ~Example()
    {
        std::cout << "dtor" << std::endl;
        delete[] ptr;
    }

    const char * getValue() const
    {
        return ptr;
    }

    void setValue(const char *value)
    {
        delete[]ptr;
        ptr = new char[strlen(value)];
        strcpy(ptr, value);
    }
private:
    char *ptr;
};

```
Класс `Example` умеет создаваться, уничтожаться, получать и устанавливаться значение поля `ptr`.
Попробуем запустить пример:
```cpp
int main()
{
    Example c1("str1");
    Example c2 = c1;

    std::cout << "c1 value " << c1.getValue() << std::endl;
    std::cout << "c2 value " << c2.getValue() << std::endl;
    return 0;
}
```

![](https://cs5.pikabu.ru/images/previews_comm/2015-03_1/14255420622577.jpg)

Запуск этого кода приведет к SIGABRT
```sh
ctor
c1 value str1
c2 value str1
dtor
dtor
Signal: SIGABRT (Aborted)
```
Почему так произошло: для создания объекта `c1` вызвался наш коструктор, с помощью оператора new выделяется память для хранения строки. Затем для создания объекта `c2` вызвался конструктор копирования по умолчанию, который за нас сгенерировал компилятор и произошло примерно такое:
```cpp
c2.ptr = c1.ptr;
```
Мы создали два объекта которые указывают на один и тот же участок памяти, а затем при удалении для каждого объекта вызывается деструктор. В деструкторе с помощью оператора `delete` происходит освобождение памяти. 
Удаляем первый объект - все ок, память была занята мы ее освободили.
Удаляем второй объект - трабла, память уже свободна, а мы ее освобождаем еще раз.

Но мы то уже умеем реализовывать конструктор копирования:
```cpp
class Example
{
public:
    Example(const char *value)
    {
        ptr = new char[strlen(value)];
        strcpy(ptr, value);
        std::cout << "ctor" << std::endl;
    }
    //конструктор копирования 
    Example(const Example &rhs)
    {
        ptr = new char[strlen(rhs.ptr)];
        strcpy(ptr, rhs.ptr);
        std::cout << "copy ctor" << std::endl;
    }
    ~Example()
    {
        std::cout << "dtor" << std::endl;
        delete[] ptr;
    }
    const char * getValue() const
    {
        return ptr;
    }
    void setValue(const char *value)
    {
        delete[]ptr;
        ptr = new char[strlen(value)];
        strcpy(ptr, value);
    }

private:
    char *ptr;
};
```

Пробуем еще раз:
```cpp
int main()
{
    Example c1("str1");
    Example c2 = c1;

    std::cout << "c1 value " << c1.getValue() << std::endl;
    std::cout << "c2 value " << c2.getValue() << std::endl;
    return 0;
}
```
Теперь программа не падает и выводит ровно то что от нее ожидается:
```sh
ctor
copy ctor
c1 value str1
c2 value str1
dtor
dtor
```
> Важно помнить, что явная реализация конструктора копирования необходима только в случае ручного управления ресурсами.

## Оператор присваивания
Но кто нас теперь остановит. Продолжим:
```cpp
int main()
{
    Example c1("str1");
    Example c2 = c1;
    
    std::cout << "c1 value " << c1.getValue() << std::endl;
    std::cout << "c2 value " << c2.getValue() << std::endl;
    
    c1 = c2;
    std::cout << "c1 value " << c1.getValue() << std::endl;
    std::cout << "c2 value " << c2.getValue() << std::endl;
    return 0;
}
```
Что же мы увидим? 
```sh
ctor
copy ctor
c1 value str1
c2 value str1
c1 value str1
c2 value str1
dtor
dtor
Signal: SIGABRT (Aborted)
```
`SIGABRT` - one love
Разберемся за что в этот раз, посмотрим внимательно на эту строчку:
```cpp
c1 = c2;
```
для того чтобы присвоить один объект другому используется оператор присваивания, для класса `Example` мы его не перегрузили, соответственно компилятор создал его за нас, вот такой он у нас сильный и независимый компилятор.
В этом в примере две серьезные проблемы:
1. Выделилось два участка памяти, для `c1` и `с2`, затем `c1` присвоили `c2`, т.е. `c1` и `c2` стали указывать на один и тот же участок в памяти, значение указателя `c1.ptr` потерялось, соответственно освободить память, которая находится по этому адресу стало просто невозможно.
2. При уничтожении объектов первый объект уничтожается успешно, а при уничтожении второго объекта получим `SIGABRT`, так как пытаемся освободить память повторно.

%username%, давай поможем компилятору ~~найти жулика~~ правильно присваивать объекты:
```cpp
class Example
{
public:
    Example(const char *value)
    {
        ptr = new char[strlen(value)];
        strcpy(ptr, value);
        std::cout << "ctor" << std::endl;
    }

    //конструктор копирования
    Example(const Example &rhs)
    {
        ptr = new char[strlen(rhs.ptr)];
        strcpy(ptr, rhs.ptr);
        std::cout << "copy ctor" << std::endl;
    }
    //оператор присваивания
    Example &operator = (const Example &rhs)
    {
        std::cout << "copy operator" << std::endl;
        delete[]ptr;
        ptr = new char[strlen(rhs.ptr)];
        strcpy(ptr, rhs.ptr);
        return *this;
    }
    
    ~Example()
    {
        std::cout << "dtor" << std::endl;
        delete[] ptr;
    }
    const char * getValue() const
    {
        return ptr;
    }
    void setValue(const char *value)
    {
        delete[]ptr;
        ptr = new char[strlen(value)];
        strcpy(ptr, value);
    }

private:
    char *ptr;
};
```
Запускаем, убеждаемся что все работает как нужно.
Выделили память два раза для каждого из объекта, удалили два раза, указатели нигде не потерялись.

### Правило трех

Мы реализовали **правило трех**, которое включает в себя:
* деструктор
```cpp
~Example()
```
* конструктор копий
```cpp
Example(const Example &rhs)
```
* оператор присваивания копий
```cpp
Example&operator = (const Example &rhs)
```

## Конструктор перемещения и оператор перемещения
Все здорово, но что если можно сделать код быстрее и эффективнее? Что если в конструкторе копирования нам придется копировать огромный временный объекта, который затем придется удалять? 
Что если мы не хотим копировать объект, а хотим переместить содержимое одного объекта в другой, не совершая дорогостоящей операции копирования?
Для этого существует **семантика перемещения**.
> Семантика перемещения позволяет компиляторам заменять дорогостоящие операции копирования менее дорогими перемещениями. Так же, как копирующие конструкторы и копирующие операторы присваивания дают вам контроль над тем, что означает копирование объектов, так и перемещающие конструкторы и перемещающие операторы присваивания предоставляют контроль над семантикой перемещения. Семантика перемещения позволяет также создавать типы, которые могут только перемещаться, такие как `std::unique_ptr`, `std::future` или `std::thread`.

То есть теперь мы можем реализовать конструктор перемещения, который вместо копирования будет перемещать (юхууу!)
```cpp
//конструктор перемещения
Example(Example && rhs)
{
    std::cout << "move ctor" << std::endl;
    ptr = rhs.ptr;
    rhs.ptr = nullptr;
}
```
код вызывающий этот конструктор:
```cpp
int main()
{
    Example c3(std::move(Example("str3")));
    std::cout << "c3 value " << c3.getValue() << std::endl;
    return 0;
}
```
вывод
```
ctor
move ctor
dtor
c3 value str3
dtor
```
Деструктор был вызван для раза, первый раз сразу после вызова конструктора перемещения - это деструктор для пустого объекта, который появился после работы конструктора перемещения, а именно строчка `rhs.ptr = nullptr`. Вызов деструктора для объекта содержащего указатель на `nullptr` безопасен.
Второй вызов деструктора соответственно для уничтожения объекта `c3` и освобождения ресурсов.

Теперь реализуем оператор перемещения:
```cpp
//оператор перемещения
Example& operator = (Example && rhs)
{
    std::cout << "move operator" << std::endl;
    char *tmp = ptr;
    ptr       = rhs.ptr;
    rhs.ptr   = tmp;
    return *this;
}
```
Пример использования:
```cpp
int main()
{
    Example c4 = Example("str4");
    Example c5 = Example("str5");
    c4 = std::move(c5);
    std::cout << "c4 value " << c4.getValue() << std::endl;
```
Вывод:
```sh
ctor
ctor
move operator
c4 value str5
dtor
dtor
```
После выполнения этого кода нельзя обращаться к объекту `c5`, его уже не существует. Грубо говоря `std::move(c5)` помечает объект как "временный" и дальнейшие операции с ним небезопасны.

Код полностью:
```cpp
#include <iostream>
#include <string.h>

class Example
{
public:

    Example(const char *value)
    {
        ptr = new char[strlen(value)];
        strcpy(ptr, value);
        std::cout << "ctor" << std::endl;
    }

    //конструктор копирования
    Example(const Example &rhs)
    {
        ptr = new char[strlen(rhs.ptr)];
        strcpy(ptr, rhs.ptr);
        std::cout << "copy ctor" << std::endl;
    }
    //конструктор перемещения
    Example(Example && rhs)
    {
        std::cout << "move ctor" << std::endl;
        ptr = rhs.ptr;
        rhs.ptr = nullptr;
    }

    //оператор присваивания копий
    Example&operator = (const Example &rhs)
    {
        std::cout << "copy operator" << std::endl;
        delete[]ptr;
        ptr = new char[strlen(rhs.ptr)];
        strcpy(ptr, rhs.ptr);
        return *this;
    }

    //оператор перемещения
    Example& operator = (Example && rhs)
    {
        std::cout << "move operator" << std::endl;
        char *tmp = ptr;
        ptr       = rhs.ptr;
        rhs.ptr   = tmp;
        return *this;
    }

    //деструктор
    ~Example()
    {
        std::cout << "dtor" << std::endl;
        delete[] ptr;
    }
    
    const char * getValue() const
    {
        return ptr;
    }

private:
    char *ptr;
};

int main()
{
    Example c1("str1");
    Example c2 = c1;

    std::cout << "c1 value " << c1.getValue() << std::endl;
    std::cout << "c2 value " << c2.getValue() << std::endl;

    c1 = c2;

    std::cout << "c1 value " << c1.getValue() << std::endl;
    std::cout << "c2 value " << c2.getValue() << std::endl;

    Example c3(std::move(Example("str3")));
    std::cout << "c3 value " << c3.getValue() << std::endl;

    Example c4 = Example("str4");
    Example c5 = Example("str5");

    c4 = std::move(c5);

    std::cout << "c4 value " << c4.getValue() << std::endl;
    return 0;
}
```

Обратите внимание:
```cpp
//конструктор копирования
Example(const Example &rhs)

//конструктор перемещения
Example(Example && rhs)
```

```cpp
//оператор присваивания копий
Example&operator = (const Example &rhs)

//оператор перемещения
Example& operator = (Example && rhs)
```
Конструктор перемещения и оператор присваивания копий принимают в качестве аргументов ссылки на **не** константные объекты, так как происходит непосредственное изменение этих объектов и использование этих объектов, после их передачи в конструктор перемещения и оператор присваивания копий **небезопасно**.

## Правило пяти

**Правило пяти** включает в себя:

* деструктор
```cpp
~Example()
```
* конструктор копий
```cpp
Example(const Example &rhs)
```
* оператор присваивания копий
```cpp
Example&operator = (const Example &rhs)
```
* конструктор перемещения
```cpp
Example(Example && rhs)
```
* оператор перемещения
```cpp
Example& operator = (Example && rhs)
```
**Правило трех** и **Правило пяти** говорят о том, что в общем случае, если возникла необходимость самостоятельного определения одной из операций копирования, перемещения или уничтожения нашего в соответствие с одной из выбранных стратегий, то скорее всего для корректной работы нужно будет определить все остальные функции тоже. 

## Правило нуля
Мартино Фернандесом предложил **правила ноля** 
> Не определяйте самостоятельно ни одну из функций пятерки, вместо этого поручите заботу о владении ресурсами специально придуманным для этого классам.

А такие специальные классы уже есть в стандартной библиотеке. Это `std::unique_ptr` и `std::shared_ptr`. Благодаря тому, что при использовании этих классов существует возможность задавать пользовательские deleter’ы, с помощью них можно реализовать большинство из стратегий владения. 
