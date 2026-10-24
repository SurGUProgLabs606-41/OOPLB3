/**
 * @file main.cpp
 * @brief Лабораторная работа №3, вариант №7.
 *
 * Программа демонстрирует классы Date и Event, конструкторы,
 * деструктор, перегрузку операторов, explicit, mutable,
 * статическое поле и обработку исключений.
 * explicit не позволяет передовать параметры не явно, то есть если в констркторе 3 параметра, надо передовать 3 параметра.
 * Если без explicit, то параметры не заполненные будут заполняться по умолчанию.
 * Перегрузка - конструкторы называются одинаково, но имеют разные параметры.
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
#include <stdexcept>
#include <type_traits>
#include "Date.h"

using namespace std;

int main() {
    try {
        cout << "Лабораторная работа №3. Вариант №7\n";

        /**
         * @brief [Задание 1] Демонстрация трёх конструкторов.
         *
         * d1 - создаётся конструктором по умолчанию,
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
        // Date d4 = 15;

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
         * @brief Создаёт массив мероприятий и выводит расписание.
         *
         * Каждый Event содержит название и объект Date.
         */
        cout << "\n8. Расписание мероприятий\n";

        Event events[] = {
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