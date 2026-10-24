/**
 * @file Date.cpp
 * @brief Реализация методов классов Date и Event.
 */
#include "Date.h"

using namespace std;

int Date::objectCount = 0;

/**
 * @brief Проверяет, является ли год високосным.
 * @param year Проверяемый год.
 * @return true, если год високосный, иначе false.
 */
bool Date::isLeapYear(int year) {
    return year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
}

/**
 * @brief Определяет количество дней в месяце.
 * @param month Номер месяца.
 * @param year Год, используемый для проверки февраля.
 * @return Количество дней в месяце.
 */
int Date::daysInMonth(int month, int year) {
    int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && isLeapYear(year)) return 29;
    return days[month - 1];
}

/**
 * @brief Проверяет корректность текущей даты.
 * @throws invalid_argument Если дата не существует.
 */
void Date::validate() const {
    if (year < 1 || month < 1 || month > 12 || day < 1 || day > daysInMonth(month, year)) {
        throw invalid_argument("Некорректная дата");
    }
}

/**
 * @brief Преобразует дату в порядковый номер дня от начала календаря.
 * @return Порядковый номер текущей даты.
 */
long long Date::toSerial() const {
    long long result = 0;
    for (int y = 1; y < year; y++) result += isLeapYear(y) ? 366 : 365;
    for (int m = 1; m < month; m++) result += daysInMonth(m, year);
    return result + day;
}

/**
 * @brief Преобразует порядковый номер дня в календарную дату.
 * @param serial Порядковый номер дня.
 * @throws out_of_range Если номер дня меньше допустимого.
 */
void Date::fromSerial(long long serial) {
    if (serial < 1) throw out_of_range("Дата выходит за допустимый диапазон");
    year = 1;
    while (true) {
        int days = isLeapYear(year) ? 366 : 365;
        if (serial <= days) break;
        serial -= days;
        year++;
    }
    month = 1;
    while (true) {
        int days = daysInMonth(month, year);
        if (serial <= days) break;
        serial -= days;
        month++;
    }
    day = static_cast<int>(serial);
}

/**
 * @brief Создаёт дату по умолчанию — 01.01.2000.
 * :: - реализация метода класса за пределами самого класса.
 */
Date::Date() : day(1), month(1), year(2000), accessCount(0) { objectCount++; }

/**
 * @brief Создаёт дату с указанными днём, месяцем и годом.
 * @param day День месяца.
 * @param month Номер месяца.
 * @param year Год.
 * @throws invalid_argument Если передана некорректная дата.
 * :: - реализация метода класса за пределами самого класса.
 */
Date::Date(int day, int month, int year)
    : day(day), month(month), year(year), accessCount(0) {
    validate();
    objectCount++;
}

/**
 * @brief Создаёт копию другой даты.
 * @param other Объект, который копируется.
 * :: - реализация метода класса за пределами самого класса.
 */
Date::Date(const Date& other)
    : day(other.day), month(other.month), year(other.year), accessCount(0) {
    objectCount++;
}

/**
 * @brief Уменьшает счётчик существующих объектов при уничтожении.
 * :: - реализация метода класса за пределами самого класса.
 */
Date::~Date() 
{ 
    objectCount--; 
}

/**
 * @brief Выводит дату в формате ДД.ММ.ГГГГ.
 */
void Date::print() const {
    accessCount++;
    if (day < 10) cout << "0";
    cout << day << ".";
    if (month < 10) cout << "0";
    cout << month << "." << year;
}

/** 
 * @brief Возвращает день месяца. 
 * @return День месяца. 
 */
int Date::getDay() const { 
    accessCount++; return day; 
}
/** 
 * @brief Возвращает номер месяца. 
 * @return Номер месяца. 
 */
int Date::getMonth() const { 
    accessCount++; return month; 
}
/** 
 * @brief Возвращает год. 
 * @return Год. 
 */
int Date::getYear() const { 
    accessCount++; return year; 
}
/** 
 * @brief Возвращает количество обращений к объекту. 
 * @return Счётчик обращений. 
 */
int Date::getAccessCount() const { 
    return accessCount; 
}
/** 
 * @brief Возвращает число созданных и ещё не уничтоженных объектов Date. 
 * @return Количество объектов. 
 */
int Date::getObjectCount() { 
    return objectCount; 
}

/**
 * @brief Определяет день недели для текущей даты.
 * @return Название дня недели.
 * :: - реализация метода класса за пределами самого класса.
 */
string Date::getWeekDay() const {
    string weekdays[] = {"Понедельник", "Вторник", "Среда", "Четверг", "Пятница", "Суббота", "Воскресенье"};
    return weekdays[(toSerial() - 1) % 7];
}

/** 
 * @brief Прибавляет указанное число дней к текущей дате. 
 * @param days Количество дней. 
 * @return Ссылка на изменённый объект. 
 */
Date& Date::operator+=(int days) { fromSerial(toSerial() + days); return *this; }
/** 
 * @brief Вычитает указанное число дней из текущей даты. 
 * @param days Количество дней. 
 * @return Ссылка на изменённый объект. 
 */
Date& Date::operator-=(int days) { fromSerial(toSerial() - days); return *this; }
/** 
 * @brief Выполняет префиксный инкремент даты. 
 * @return Ссылка на изменённый объект. 
 */
Date& Date::operator++() { 
    *this += 1; 
    return *this; 
}
/** 
 * @brief Выполняет постфиксный инкремент даты. 
 * @return Копия даты до изменения. 
 */
Date Date::operator++(int) { 
    Date old(*this); ++(*this); 
    return old; 
}
/** 
 * @brief Выполняет префиксный декремент даты. 
 * @return Ссылка на изменённый объект. 
 */
Date& Date::operator--() { 
    *this -= 1; return *this; 
}
/** 
 * @brief Выполняет постфиксный декремент даты. 
 * @return Копия даты до изменения. 
 */
Date Date::operator--(int) { 
    Date old(*this); --(*this); return old; 
}
/** 
 * @brief Умножает порядковый номер даты на число. 
 * @param value Множитель. 
 * @return Ссылка на изменённый объект. 
 */
Date& Date::operator*=(int value) { 
    fromSerial(toSerial() * value); 
    return *this; 
}
/** 
 * @brief Делит порядковый номер даты на число. 
 * @param value Делитель. @return Ссылка на изменённый объект. 
 * @throws invalid_argument При делении на ноль. 
 */
Date& Date::operator/=(int value) {
    if (value == 0) throw invalid_argument("Деление на ноль");
    fromSerial(toSerial() / value);
    return *this;
}

/** 
 * @brief Возвращает дату, сдвинутую вперёд на заданное число дней. 
 * @param lhs Исходная дата. 
 * @param days Число дней. 
 * @return Новая дата. 
 */
Date operator+(Date lhs, int days) { lhs += days; return lhs; }
/** 
 * @brief Возвращает дату, сдвинутую назад на заданное число дней. 
 * @param lhs Исходная дата. 
 * @param days Число дней. 
 * @return Новая дата. */
Date operator-(Date lhs, int days) { lhs -= days; return lhs; }
/** 
 * @brief Умножает порядковый номер даты и возвращает результат. 
 * @param lhs Исходная дата. 
 * @param value Множитель. 
 * @return Новая дата. 
 */
Date operator*(Date lhs, int value) { lhs *= value; return lhs; }
/** 
 * @brief Делит порядковый номер даты и возвращает результат. 
 * @param lhs Исходная дата. 
 * @param value Делитель. 
 * @return Новая дата. */
Date operator/(Date lhs, int value) { lhs /= value; return lhs; }
/**
 * @brief Вычисляет разность двух дат в днях.
 * @param other Другая дата.
 * @return Разность порядковых номеров дат в днях.
 *
 * Date - Date возвращает число дней, тогда как Date - int
 * возвращает объект Date.
 */
long long Date::operator-(const Date& other) const {
    return toSerial() - other.toSerial();
}

/**
 * @brief Проверяет, что текущая дата раньше другой.
 * @param other Другая дата для сравнения.
 * @return true, если текущая дата раньше other; иначе false.
 *
 * [Задание 4] Оператор сравнения <.
 */
bool Date::operator<(const Date& other) const {
    return toSerial() < other.toSerial();
}

/**
 * @brief Проверяет, что текущая дата позже другой.
 * @param other Другая дата для сравнения.
 * @return true, если текущая дата позже other; иначе false.
 *
 * [Задание 4] Оператор сравнения >.
 */
bool Date::operator>(const Date& other) const {
    return toSerial() > other.toSerial();
}

/**
 * @brief Проверяет, что текущая дата раньше или равна другой.
 * @param other Другая дата для сравнения.
 * @return true, если текущая дата меньше или равна other; иначе false.
 *
 * [Задание 4] Оператор сравнения <=.
 */
bool Date::operator<=(const Date& other) const {
    return toSerial() <= other.toSerial();
}

/**
 * @brief Проверяет, что текущая дата позже или равна другой.
 * @param other Другая дата для сравнения.
 * @return true, если текущая дата больше или равна other; иначе false.
 *
 * [Задание 4] Оператор сравнения >=.
 */
bool Date::operator>=(const Date& other) const {
    return toSerial() >= other.toSerial();
}

/**
 * @brief Проверяет равенство двух дат.
 * @param other Другая дата для сравнения.
 * @return true, если даты совпадают; иначе false.
 *
 * [Задание 4] Оператор сравнения ==.
 */
bool Date::operator==(const Date& other) const {
    return toSerial() == other.toSerial();
}

/**
 * @brief Проверяет неравенство двух дат.
 * @param other Другая дата для сравнения.
 * @return true, если даты различаются; иначе false.
 *
 * [Задание 4] Оператор сравнения !=.
 */
bool Date::operator!=(const Date& other) const {
    return toSerial() != other.toSerial();
}

/**
 * @brief Создаёт мероприятие с заданным названием и датой.
 * @param name Название мероприятия.
 * @param date Дата проведения.
 */
Event::Event(const string& name, const Date& date) : name(name), date(date) {}

/**
 * @brief Выводит название мероприятия, дату и день недели.
 */
void Event::print() const {
    cout << name << " — ";
    date.print();
    cout << ", " << date.getWeekDay() << endl;
}
