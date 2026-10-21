#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <type_traits>

using namespace std;

class Date {
private:
    int day;
    int month;
    int year;

    mutable int accessCount; // Изменяемый счетчик обращений
    static int objectCount;  // Количество существующих объектов

    // Проверяем, является ли год високосным
    static bool isLeapYear(int year) {
        return year % 400 == 0 ||
               (year % 4 == 0 && year % 100 != 0);
    }

    // Возвращаем количество дней в месяце
    static int daysInMonth(int month, int year) {
        int days[] = {
            31, 28, 31, 30, 31, 30,
            31, 31, 30, 31, 30, 31
        };

        if (month == 2 && isLeapYear(year)) {
            return 29;
        }

        return days[month - 1];
    }

    // Проверяем корректность даты
    void validate() const {
        if (year < 1 || month < 1 || month > 12 ||
            day < 1 || day > daysInMonth(month, year)) {
            throw invalid_argument("Некорректная дата");
        }
    }

    // Преобразуем дату в порядковый номер дня
    long long toSerial() const {
        long long result = 0;

        for (int y = 1; y < year; y++) {
            result += isLeapYear(y) ? 366 : 365;
        }

        for (int m = 1; m < month; m++) {
            result += daysInMonth(m, year);
        }

        return result + day;
    }

    // Преобразуем порядковый номер обратно в дату
    void fromSerial(long long serial) {
        if (serial < 1) {
            throw out_of_range("Дата выходит за допустимый диапазон");
        }

        year = 1;

        while (true) {
            int days = isLeapYear(year) ? 366 : 365;

            if (serial <= days) {
                break;
            }

            serial -= days;
            year++;
        }

        month = 1;

        while (true) {
            int days = daysInMonth(month, year);

            if (serial <= days) {
                break;
            }

            serial -= days;
            month++;
        }

        day = static_cast<int>(serial);
    }

public:
    // 1. Конструктор по умолчанию
    Date() : day(1), month(1), year(2000), accessCount(0) {
        objectCount++;
    }

    // 2. Параметризованный конструктор
    // explicit запрещает неявное преобразование int в Date
    explicit Date(int day, int month = 1, int year = 2000)
        : day(day), month(month), year(year), accessCount(0) {
        validate();
        objectCount++;
    }

    // 3. Конструктор копирования
    Date(const Date& other)
        : day(other.day),
          month(other.month),
          year(other.year),
          accessCount(0) {
        objectCount++;
    }

    // Деструктор уменьшает количество существующих объектов
    ~Date() {
        objectCount--;
    }

    // Вывод даты
    void print() const {
        accessCount++;

        if (day < 10) cout << "0";
        cout << day << ".";

        if (month < 10) cout << "0";
        cout << month << "." << year;
    }

    // Получение полей даты
    int getDay() const {
        accessCount++;
        return day;
    }

    int getMonth() const {
        accessCount++;
        return month;
    }

    int getYear() const {
        accessCount++;
        return year;
    }

    // Получение счетчика обращений
    int getAccessCount() const {
        return accessCount;
    }

    // Получение количества существующих объектов
    static int getObjectCount() {
        return objectCount;
    }

    // Получение дня недели
    string getWeekDay() const {
        string weekdays[] = {
            "Понедельник", "Вторник", "Среда",
            "Четверг", "Пятница", "Суббота", "Воскресенье"
        };

        return weekdays[(toSerial() - 1) % 7];
    }

    // Арифметические операторы присваивания
    Date& operator+=(int days) {
        fromSerial(toSerial() + days);
        return *this;
    }

    Date& operator-=(int days) {
        fromSerial(toSerial() - days);
        return *this;
    }

    // Префиксный инкремент: следующий день
    Date& operator++() {
        *this += 1;
        return *this;
    }

    // Постфиксный инкремент: возвращает старую дату
    Date operator++(int) {
        Date old(*this);
        ++(*this);
        return old;
    }

    // Префиксный декремент: предыдущий день
    Date& operator--() {
        *this -= 1;
        return *this;
    }

    // Постфиксный декремент: возвращает старую дату
    Date operator--(int) {
        Date old(*this);
        --(*this);
        return old;
    }
};

// Инициализация статического поля
int Date::objectCount = 0;

int main() {
    try {
        cout << "Лабораторная работа №3. Вариант №7\n";

        // 1. Три конструктора
        cout << "\n1. Конструкторы\n";

        Date d1;
        Date d2(20, 10, 2026);
        Date d3(d2);

        cout << "Конструктор по умолчанию: ";
        d1.print();

        cout << "\nПараметризованный конструктор: ";
        d2.print();

        cout << "\nКонструктор копирования: ";
        d3.print();
        cout << endl;

        // 2. Демонстрация explicit
        cout << "\n2. explicit\n";

        Date d4(15);

        cout << "Явное создание Date(15): ";
        d4.print();
        cout << endl;

        cout << "Неявное преобразование int в Date разрешено: "
             << boolalpha
             << is_convertible<int, Date>::value << endl;

        // Следующая строка вызовет ошибку компиляции:
        // Date d5 = 15;
    }
    catch (const exception& e) {
        cerr << "\nОшибка: " << e.what() << endl;
        return 1;
    }

    return 0;
}