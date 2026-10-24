/**
 * @file Date.h
 * @brief Объявления классов Date и Event.
 */
#ifndef DATE_H
#define DATE_H

#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;

/**
 * @class Date
 * @brief Класс для хранения и обработки календарной даты.
 * Дата хранится в полях день, месяц и год.
 */
class Date {
private:
    /// День месяца.
    int day;
    /// Номер месяца от 1 до 12.
    int month;
    /// Год.
    int year;

    /**
     * @brief Счётчик обращений к объекту.
     *
     * [Задания 5, 10] mutable — спецификатор, который разрешает
     * изменять это поле даже внутри const-методов и у const-объекта.
     */
    mutable int accessCount;

    /**
     * @brief Количество существующих объектов Date.
     *
     * [Задание 6] static означает, что поле общее для всех
     * объектов класса, а не отдельное для каждого объекта.
     */
    static int objectCount;

    /// Проверяет, является ли год високосным.
    static bool isLeapYear(int year);
    /// Возвращает количество дней в указанном месяце.
    static int daysInMonth(int month, int year);
    /// Проверяет корректность даты.
    void validate() const;
    /// Преобразует дату в порядковый номер дня.
    long long toSerial() const;
    /// Преобразует порядковый номер дня обратно в дату.
    void fromSerial(long long serial);

public:
    /// Конструктор по умолчанию: 01.01.2000.
    Date();
    /// Параметризованный конструктор; explicit запрещает неявное преобразование int в Date.
    explicit Date(int day, int month = 1, int year = 2000);
    /// Конструктор копирования.
    Date(const Date& other);
    /// Деструктор класса Date.
    ~Date();

    /// Выводит дату в формате ДД.ММ.ГГГГ.
    void print() const;
    /// Возвращает день месяца.
    int getDay() const;
    /// Возвращает номер месяца.
    int getMonth() const;
    /// Возвращает год.
    int getYear() const;
    /// Возвращает количество обращений к объекту.
    int getAccessCount() const;
    /// Возвращает количество существующих объектов Date.
    static int getObjectCount();
    /// Определяет день недели.
    string getWeekDay() const;

    /// Прибавляет к дате заданное количество дней.
    Date& operator+=(int days);
    /// Вычитает из даты заданное количество дней.
    Date& operator-=(int days);
    /// Префиксный инкремент.
    Date& operator++();
    /// Постфиксный инкремент.
    Date operator++(int);
    /// Префиксный декремент.
    Date& operator--();
    /// Постфиксный декремент.
    Date operator--(int);
    /// Умножает порядковый номер даты на число.
    Date& operator*=(int value);
    /// Делит порядковый номер даты на число.
    Date& operator/=(int value);

    /// Прибавляет дни и возвращает новую дату.
    friend Date operator+(Date lhs, int days);
    /// Вычитает дни и возвращает новую дату.
    friend Date operator-(Date lhs, int days);
    /// Умножает порядковый номер даты.
    friend Date operator*(Date lhs, int value);
    /// Делит порядковый номер даты.
    friend Date operator/(Date lhs, int value);
    /**
     * @brief Вычисляет разность двух дат в днях.
     * @param other Другая дата.
     * @return Разность порядковых номеров дат в днях.
     *
     * Date - Date возвращает число дней, тогда как Date - int
     * возвращает объект Date.
     */
    long long operator-(const Date& other) const;

    /**
     * @brief Проверяет, что текущая дата раньше другой.
     * @param other Другая дата для сравнения.
     * @return true, если текущая дата раньше other; иначе false.
     *
     * [Задание 4] Оператор сравнения <.
     */
    bool operator<(const Date& other) const;

    /**
     * @brief Проверяет, что текущая дата позже другой.
     * @param other Другая дата для сравнения.
     * @return true, если текущая дата позже other; иначе false.
     *
     * [Задание 4] Оператор сравнения >.
     */
    bool operator>(const Date& other) const;

    /**
     * @brief Проверяет, что текущая дата раньше или равна другой.
     * @param other Другая дата для сравнения.
     * @return true, если текущая дата меньше или равна other; иначе false.
     *
     * [Задание 4] Оператор сравнения <=.
     */
    bool operator<=(const Date& other) const;

    /**
     * @brief Проверяет, что текущая дата позже или равна другой.
     * @param other Другая дата для сравнения.
     * @return true, если текущая дата больше или равна other; иначе false.
     *
     * [Задание 4] Оператор сравнения >=.
     */
    bool operator>=(const Date& other) const;

    /**
     * @brief Проверяет равенство двух дат.
     * @param other Другая дата для сравнения.
     * @return true, если даты совпадают; иначе false.
     *
     * [Задание 4] Оператор сравнения ==.
     */
    bool operator==(const Date& other) const;

    /**
     * @brief Проверяет неравенство двух дат.
     * @param other Другая дата для сравнения.
     * @return true, если даты различаются; иначе false.
     *
     * [Задание 4] Оператор сравнения !=.
     */
    bool operator!=(const Date& other) const;
};

/**
 * @class Event
 * @brief Мероприятие с названием и датой проведения.
 */
class Event {
public:
    /// Название мероприятия.
    string name;
    /// Дата проведения мероприятия.
    Date date;

    /// Создаёт мероприятие.
    Event(const string& name, const Date& date);
    /// Выводит название, дату и день недели мероприятия.
    void print() const;
};

#endif
