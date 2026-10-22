/**
 * @file main.cpp
 * @brief Лабораторная работа №3, вариант №7.
 *
 * Программа демонстрирует классы Date и Event, конструкторы,
 * деструктор, перегрузку операторов, explicit, mutable,
 * статическое поле и обработку исключений.
 *
 * Выполненные задания:
 * 1. Три конструктора.
 * 2. Префиксные и постфиксные ++ и --.
 * 3. Операторы +=, -=, *=, /=.
 * 4. Бинарные арифметические операторы и сравнения.
 * 5. Использование explicit и mutable.
 * 6. Одно статическое поле.
 * 7. Пользовательский деструктор.
 * 8. Демонстрация операторов в main().
 * 9. Демонстрация запрета неявного преобразования.
 * 10. Изменение mutable-поля константного объекта.
 */

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <type_traits>

using namespace std;

/**
 * @class Date
 * @brief Класс для хранения и обработки календарной даты.
 *
 * Дата хранится в трёх полях: день, месяц и год.
 * Для арифметических операций дата преобразуется в порядковый
 * номер дня, после чего результат переводится обратно в дату.
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

    /**
     * @brief Проверяет, является ли год високосным.
     * @param year Проверяемый год.
     * @return true, если год високосный; иначе false.
     *
     * Год високосный, если делится на 400 либо делится на 4,
     * но не делится на 100.
     */
    static bool isLeapYear(int year) {
        return year % 400 == 0 ||
               (year % 4 == 0 && year % 100 != 0);
    }

    /**
     * @brief Возвращает количество дней в указанном месяце.
     * @param month Номер месяца от 1 до 12.
     * @param year Год.
     * @return Количество дней в месяце.
     *
     * В феврале високосного года 29 дней.
     */
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

    /**
     * @brief Проверяет корректность текущей даты.
     * @throws invalid_argument Если дата некорректна.
     *
     * const означает, что метод не изменяет обычные поля объекта.
     */
    void validate() const {
        if (year < 1 || month < 1 || month > 12 ||
            day < 1 || day > daysInMonth(month, year)) {
            throw invalid_argument("Некорректная дата");
        }
    }

    /**
     * @brief Преобразует дату в порядковый номер дня.
     * @return Порядковый номер даты, начиная с 01.01.0001.
     *
     * Подсчитываются дни предыдущих лет, предыдущих месяцев
     * текущего года и текущий день.
     */
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

    /**
     * @brief Преобразует порядковый номер дня обратно в дату.
     * @param serial Порядковый номер дня, начиная с 1.
     * @throws out_of_range Если serial меньше 1.
     *
     * Сначала определяется год, затем месяц, затем день.
     */
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
    /**
     * @brief Конструктор по умолчанию.
     *
     * [Задание 1] Создаёт дату 01.01.2000.
     * Инициализирует счётчик обращений и увеличивает objectCount.
     */
    Date() : day(1), month(1), year(2000), accessCount(0) {
        objectCount++;
    }

    /**
     * @brief Параметризованный конструктор.
     * @param day День месяца.
     * @param month Месяц; по умолчанию 1.
     * @param year Год; по умолчанию 2000.
     * @throws invalid_argument Если дата некорректна.
     *
     * [Задания 1, 5, 9] explicit запрещает неявное преобразование
     * int в Date. Например, Date d(15) разрешено, а Date d = 15 —
     * запрещено, поскольку это неявное преобразование.
     */
    explicit Date(int day, int month = 1, int year = 2000)
        : day(day), month(month), year(year), accessCount(0) {
        validate();
        objectCount++;
    }

    /**
     * @brief Конструктор копирования.
     * @param other Объект, из которого копируется дата.
     *
     * [Задание 1] Создаёт новый объект с такими же днём,
     * месяцем и годом. Счётчик обращений начинает с нуля.
     */
    Date(const Date& other)
        : day(other.day),
          month(other.month),
          year(other.year),
          accessCount(0) {
        objectCount++;
    }

    /**
     * @brief Деструктор класса Date.
     *
     * [Задание 7] Пользовательский деструктор уменьшает количество
     * существующих объектов. Вызывается автоматически при завершении
     * времени жизни объекта.
     */
    ~Date() {
        objectCount--;
    }

    /**
     * @brief Выводит дату в формате ДД.ММ.ГГГГ.
     *
     * Увеличивает счётчик обращений. Метод const не меняет саму дату,
     * а изменение accessCount разрешено благодаря mutable.
     */
    void print() const {
        accessCount++;

        if (day < 10) cout << "0";
        cout << day << ".";

        if (month < 10) cout << "0";
        cout << month << "." << year;
    }

    /**
     * @brief Возвращает день месяца.
     * @return День месяца.
     *
     * Увеличивает счётчик обращений.
     */
    int getDay() const {
        accessCount++;
        return day;
    }

    /**
     * @brief Возвращает номер месяца.
     * @return Номер месяца.
     *
     * Увеличивает счётчик обращений.
     */
    int getMonth() const {
        accessCount++;
        return month;
    }

    /**
     * @brief Возвращает год.
     * @return Год.
     *
     * Увеличивает счётчик обращений.
     */
    int getYear() const {
        accessCount++;
        return year;
    }

    /**
     * @brief Возвращает количество обращений к объекту.
     * @return Текущее значение accessCount.
     *
     * Сам счётчик при вызове метода не увеличивается.
     */
    int getAccessCount() const {
        return accessCount;
    }

    /**
     * @brief Возвращает количество существующих объектов Date.
     * @return Значение статического счётчика.
     *
     * [Задание 6] Метод static можно вызвать без объекта:
     * Date::getObjectCount().
     */
    static int getObjectCount() {
        return objectCount;
    }

    /**
     * @brief Определяет день недели для даты.
     * @return Название дня недели на русском языке.
     *
     * Порядковый номер преобразуется в индекс массива от 0 до 6.
     * В используемой системе 01.01.0001 соответствует понедельнику.
     */
    string getWeekDay() const {
        string weekdays[] = {
            "Понедельник", "Вторник", "Среда",
            "Четверг", "Пятница", "Суббота", "Воскресенье"
        };

        return weekdays[(toSerial() - 1) % 7];
    }

    /**
     * @brief Прибавляет к дате заданное количество дней.
     * @param days Количество прибавляемых дней.
     * @return Ссылка на текущий объект.
     *
     * [Задание 3] Оператор += изменяет текущий объект.
     */
    Date& operator+=(int days) {
        fromSerial(toSerial() + days);
        return *this;
    }

    /**
     * @brief Вычитает из даты заданное количество дней.
     * @param days Количество вычитаемых дней.
     * @return Ссылка на текущий объект.
     *
     * [Задание 3] Оператор -= изменяет текущий объект.
     */
    Date& operator-=(int days) {
        fromSerial(toSerial() - days);
        return *this;
    }

    /**
     * @brief Префиксный инкремент даты.
     * @return Ссылка на изменённый объект.
     *
     * [Задание 2] ++date переводит дату на следующий день
     * и возвращает уже изменённый объект.
     */
    Date& operator++() {
        *this += 1;
        return *this;
    }

    /**
     * @brief Постфиксный инкремент даты.
     * @return Копия даты до увеличения.
     *
     * [Задание 2] date++ сохраняет старую дату, увеличивает
     * текущую и возвращает сохранённую копию.
     * Параметр int отличает постфиксную форму от префиксной.
     */
    Date operator++(int) {
        Date old(*this);
        ++(*this);
        return old;
    }

    /**
     * @brief Префиксный декремент даты.
     * @return Ссылка на изменённый объект.
     *
     * [Задание 2] --date переводит дату на предыдущий день.
     */
    Date& operator--() {
        *this -= 1;
        return *this;
    }

    /**
     * @brief Постфиксный декремент даты.
     * @return Копия даты до уменьшения.
     *
     * [Задание 2] date-- сохраняет старую дату, уменьшает
     * текущую и возвращает сохранённую копию.
     */
    Date operator--(int) {
        Date old(*this);
        --(*this);
        return old;
    }

    /**
     * @brief Умножает порядковый номер даты на число.
     * @param value Множитель.
     * @return Ссылка на текущий объект.
     *
     * [Задание 3] Оператор *= изменяет дату. Это демонстрационная
     * арифметическая операция, а не обычное календарное умножение.
     */
    Date& operator*=(int value) {
        fromSerial(toSerial() * value);
        return *this;
    }

    /**
     * @brief Делит порядковый номер даты на число.
     * @param value Делитель.
     * @return Ссылка на текущий объект.
     * @throws invalid_argument Если делитель равен нулю.
     *
     * [Задание 3] Оператор /= использует целочисленное деление.
     */
    Date& operator/=(int value) {
        if (value == 0) {
            throw invalid_argument("Деление на ноль");
        }

        fromSerial(toSerial() / value);
        return *this;
    }

    /**
     * @brief Прибавляет дни и возвращает новую дату.
     * @param lhs Копия исходной даты.
     * @param days Количество прибавляемых дней.
     * @return Новая дата.
     *
     * [Задание 4] Бинарный оператор + не изменяет исходный объект:
     * он получает копию, изменяет её и возвращает результат.
     * friend разрешает функции доступ к закрытым членам класса.
     */
    friend Date operator+(Date lhs, int days) {
        lhs += days;
        return lhs;
    }

    /**
     * @brief Вычитает дни и возвращает новую дату.
     * @param lhs Копия исходной даты.
     * @param days Количество вычитаемых дней.
     * @return Новая дата.
     *
     * [Задание 4] Бинарный оператор - для Date и int.
     */
    friend Date operator-(Date lhs, int days) {
        lhs -= days;
        return lhs;
    }

    /**
     * @brief Умножает порядковый номер даты.
     * @param lhs Копия исходной даты.
     * @param value Множитель.
     * @return Новая дата.
     *
     * [Задание 4] Бинарный оператор * не изменяет исходную дату.
     */
    friend Date operator*(Date lhs, int value) {
        lhs *= value;
        return lhs;
    }

    /**
     * @brief Делит порядковый номер даты.
     * @param lhs Копия исходной даты.
     * @param value Делитель.
     * @return Новая дата.
     * @throws invalid_argument Если делитель равен нулю.
     *
     * [Задание 4] Бинарный оператор / не изменяет исходную дату.
     */
    friend Date operator/(Date lhs, int value) {
        lhs /= value;
        return lhs;
    }

    /**
     * @brief Вычисляет разность двух дат в днях.
     * @param other Другая дата.
     * @return Разность порядковых номеров в днях.
     *
     * [Задание 4] Date - Date возвращает число дней,
     * тогда как Date - int возвращает объект Date.
     */
    long long operator-(const Date& other) const {
        return toSerial() - other.toSerial();
    }

    /**
     * @brief Проверяет, что текущая дата раньше другой.
     * @param other Другая дата.
     * @return true, если текущая дата раньше other.
     *
     * [Задание 4] Оператор сравнения <.
     */
    bool operator<(const Date& other) const {
        return toSerial() < other.toSerial();
    }

    /**
     * @brief Проверяет, что текущая дата позже другой.
     * @param other Другая дата.
     * @return true, если текущая дата позже other.
     *
     * [Задание 4] Оператор сравнения >.
     */
    bool operator>(const Date& other) const {
        return toSerial() > other.toSerial();
    }

    /**
     * @brief Проверяет, что дата раньше или совпадает с другой.
     * @param other Другая дата.
     * @return true, если текущая дата меньше или равна other.
     *
     * [Задание 4] Оператор сравнения <=.
     */
    bool operator<=(const Date& other) const {
        return toSerial() <= other.toSerial();
    }

    /**
     * @brief Проверяет, что дата позже или совпадает с другой.
     * @param other Другая дата.
     * @return true, если текущая дата больше или равна other.
     *
     * [Задание 4] Оператор сравнения >=.
     */
    bool operator>=(const Date& other) const {
        return toSerial() >= other.toSerial();
    }

    /**
     * @brief Проверяет равенство дат.
     * @param other Другая дата.
     * @return true, если даты совпадают.
     *
     * [Задание 4] Оператор сравнения ==.
     */
    bool operator==(const Date& other) const {
        return toSerial() == other.toSerial();
    }

    /**
     * @brief Проверяет неравенство дат.
     * @param other Другая дата.
     * @return true, если даты различаются.
     *
     * [Задание 4] Оператор сравнения !=.
     */
    bool operator!=(const Date& other) const {
        return toSerial() != other.toSerial();
    }
};

/**
 * @brief Определение и начальная инициализация статического поля.
 *
 * [Задание 6] Статическое поле объявлено в классе,
 * а определено за его пределами.
 */
int Date::objectCount = 0;

/**
 * @class Event
 * @brief Мероприятие с названием и датой проведения.
 *
 * Класс содержит объект Date. Это пример композиции:
 * объект одного класса хранится в качестве поля другого класса.
 */
class Event {
public:
    /// Название мероприятия.
    string name;

    /// Дата проведения мероприятия.
    Date date;

    /**
     * @brief Создаёт мероприятие.
     * @param name Название мероприятия.
     * @param date Дата проведения.
     */
    Event(const string& name, const Date& date)
        : name(name), date(date) {}

    /**
     * @brief Выводит название, дату и день недели мероприятия.
     */
    void print() const {
        cout << name << " — ";
        date.print();
        cout << ", " << date.getWeekDay() << endl;
    }
};

/**
 * @brief Демонстрирует возможности классов Date и Event.
 *
 * [Задание 8] Проверяется работа конструкторов, преобразований,
 * унарных, арифметических и сравнительных операторов.
 * [Задание 9] Демонстрируется запрет неявного преобразования с explicit.
 * [Задание 10] Проверяется изменение mutable-поля const-объекта.
 *
 * @return 0 при успешном выполнении; 1 при обработанном исключении.
 */
int main() {
    try {
        cout << "Лабораторная работа №3. Вариант №7\n";

        /**
         * @brief [Задание 1] Демонстрация трёх конструкторов.
         *
         * d1 создаётся конструктором по умолчанию,
         * d2 — параметризованным конструктором,
         * d3 — конструктором копирования.
         */
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

        /**
         * @brief [Задания 5 и 9] Демонстрация explicit.
         *
         * Date d4(15) разрешено: конструктор вызывается явно.
         * Date d5 = 15 запрещено, поскольку explicit блокирует
         * неявное преобразование int в Date.
         */
        cout << "\n2. explicit\n";

        Date d4(15);

        cout << "Явное создание Date(15): ";
        d4.print();
        cout << endl;

        cout << "Неявное преобразование int в Date разрешено: "
             << boolalpha
             << is_convertible<int, Date>::value << endl;

        // Эта строка не компилируется из-за explicit:
        // Date d5 = 15;

        /**
         * @brief [Задание 2] Демонстрация ++ и --.
         *
         * Префиксная форма возвращает изменённую дату,
         * постфиксная — копию даты до изменения.
         */
        cout << "\n3. Унарные операторы\n";

        Date d5(20, 10, 2026);

        cout << "Исходная дата: ";
        d5.print();

        cout << "\nПрефиксный ++: ";
        (++d5).print();

        cout << "\nПостфиксный ++ (старая дата): ";
        (d5++).print();

        cout << "\nДата после постфиксного ++: ";
        d5.print();

        cout << "\nПрефиксный --: ";
        (--d5).print();

        cout << "\nПостфиксный -- (старая дата): ";
        (d5--).print();

        cout << "\nДата после постфиксного --: ";
        d5.print();
        cout << endl;

        /**
         * @brief [Задание 3] Демонстрация +=, -=, *= и /=.
         */
        cout << "\n4. Операторы +=, -=, *=, /=\n";

        Date a(10, 10, 2026);
        Date b(a);

        a += 5;
        cout << "После += 5: ";
        a.print();

        a -= 2;
        cout << "\nПосле -= 2: ";
        a.print();

        b *= 2;
        cout << "\nПосле *= 2: ";
        b.print();

        b /= 2;
        cout << "\nПосле /= 2: ";
        b.print();
        cout << endl;

        /**
         * @brief [Задание 4] Демонстрация бинарных арифметических операторов.
         *
         * Операторы +, -, * и / возвращают результат.
         * Исходный объект c не изменяется.
         */
        cout << "\n5. Операторы +, -, *, /\n";

        Date c(10, 10, 2026);

        cout << "c + 5: ";
        (c + 5).print();

        cout << "\nc - 3: ";
        (c - 3).print();

        cout << "\nc * 2: ";
        (c * 2).print();

        cout << "\nc / 2: ";
        (c / 2).print();

        cout << "\nРазность дат: "
             << (Date(20, 10, 2026) - Date(10, 10, 2026))
             << " дней\n";

        /**
         * @brief [Задание 4] Демонстрация операторов сравнения.
         */
        cout << "\n6. Операторы сравнения\n";

        Date first(10, 10, 2026);
        Date second(20, 10, 2026);
        Date same(10, 10, 2026);

        cout << "first < second: " << (first < second) << endl;
        cout << "first > second: " << (first > second) << endl;
        cout << "first <= second: " << (first <= second) << endl;
        cout << "first >= second: " << (first >= second) << endl;
        cout << "first == same: " << (first == same) << endl;
        cout << "first != second: " << (first != second) << endl;

        /**
         * @brief [Задания 5 и 10] Демонстрация mutable у const-объекта.
         *
         * constDate нельзя изменять обычными операциями.
         * Однако print(), getDay(), getMonth() и getYear()
         * увеличивают accessCount, потому что поле объявлено mutable.
         */
        cout << "\n7. mutable\n";

        const Date constDate(15, 10, 2026);

        cout << "Константная дата: ";
        constDate.print();

        cout << "\nДень: " << constDate.getDay();
        cout << "\nМесяц: " << constDate.getMonth();
        cout << "\nГод: " << constDate.getYear();

        cout << "\nСчетчик обращений: "
             << constDate.getAccessCount() << endl;

        /**
         * @brief Создаёт список мероприятий и выводит расписание.
         *
         * Каждый Event содержит название и объект Date.
         */
        cout << "\n8. Расписание мероприятий\n";

        vector<Event> events = {
            Event("Лекция", Date(5, 10, 2026)),
            Event("Практическая работа", Date(10, 10, 2026)),
            Event("Экзамен", Date(20, 10, 2026)),
            Event("Конференция", Date(15, 11, 2026)),
            Event("Зачет", Date(25, 10, 2026))
        };

        for (const Event& event : events) {
            event.print();
        }

        /**
         * @brief Подсчитывает мероприятия за октябрь и за весь 2026 год.
         */
        int monthCount = 0;
        int yearCount = 0;

        for (const Event& event : events) {
            if (event.date.getMonth() == 10 &&
                event.date.getYear() == 2026) {
                monthCount++;
            }

            if (event.date.getYear() == 2026) {
                yearCount++;
            }
        }

        cout << "\nМероприятий за октябрь 2026: "
             << monthCount << endl;

        cout << "Мероприятий за 2026 год: "
             << yearCount << endl;

        /**
         * @brief [Задание 6] Выводит количество существующих объектов Date.
         *
         * Учитываются объекты, которые существуют в этой точке программы,
         * включая поля date объектов Event.
         */
        cout << "\nКоличество существующих объектов Date: "
             << Date::getObjectCount() << endl;
    }
    /**
     * @brief Обрабатывает исключения, унаследованные от std::exception.
     * @param e Пойманное исключение.
     */
    catch (const exception& e) {
        cerr << "\nОшибка: " << e.what() << endl;
        return 1;
    }

    return 0;
}