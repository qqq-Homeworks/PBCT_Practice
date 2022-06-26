#include <iostream>
#include <iomanip>
#include "dbmsLib.h"
using namespace std;

int menu()
{
    cout << "================= МАКЕТ СУБД ===================\n";
    cout << "\t1 - Чтение таблицы из файла\n";
    cout << "\t2 - Печать таблицы\n";
    cout << "\t3 - Запись таблицы в файл\n";
    cout << "\t4 - Выдать книгу студенту\n";
    cout << "\t5 - Выход\n";
    int choice;
    cout << "Выберите действие\n";
    cin >> choice;
    while (cin.fail())
    {
        cout << "Ошибка ввода. Повторите ввод\n";
        cin.clear();
        cin.ignore(10, '\n');
        cin >> choice;
    }
    return choice;
}

//=================================================
int main()
{
    system("chcp 1251>nul");
    int StudentID = 0;
    int BookID = 0;
    int SubjectID = 0;
    char buff[20];
    char buff_subjectID[20];
    char buff_bookID[20];
    void *adress;
    dbmsLib::Row row;
    string name;
    string group;
    string tmp_date;
    dbmsLib::DBTableTxt stud("Students.txt");
    dbmsLib::DBTableTxt book("Books.txt");
    dbmsLib::DBTableTxt abonements("Abonements.txt");

    while (true)
    {
        row.clear();
        switch (menu())
        {
        case 1:
            stud.ReadDBTable("Students.txt");
            cout << "Файл с информацией о студентах успешно прочитан" << endl;
            book.ReadDBTable("Books.txt");
            cout << "Файл с информацией о книгах успешно прочитан" << endl;
            abonements.ReadDBTable("Abonements.txt");
            cout << "Файл с информацией об абонементах успешно прочитан" << endl;
        case 2:
            stud.PrintTable(80);
            book.PrintTable(80);
            abonements.PrintTable(80);
            break;
        case 3:
            stud.WriteDBTable("Students.txt");
            book.WriteDBTable("Books.txt");
            abonements.WriteDBTable("Abonements.txt");
            cout << "Информация записана в файлы" << endl;
            break;

        case 4:
        {
            bool flag = false;
            cout << "Введите ID студента" << endl;
            cin >> StudentID;
            _itoa_s(StudentID, buff, 20, 10);
            for (size_t i = 0; i < stud.GetSize(); ++i)
            {
                if (dbmsLib::comparator(dbmsLib::Int32, (stud.GetRow(i)).find("StudentID")->second, dbmsLib::Equal,
                                        dbmsLib::GetValue((char *)buff, (string) "StudentID", stud.GetHeader())))
                {
                }
            }
            if (flag)
            {
                flag = false;
                cout << "Введите дату выдачи книги(дд.мм.гггг)" << endl;
                cin >> tmp_date;
                cout << "Введите ID книги" << endl;
                cin >> BookID;
                int quantity = 0;
                int number = 0;
                string tmp_num;
                string bookIdTemp;

                _itoa_s(BookID, buff_bookID, 20, 10);

                for (size_t i = 0; i < book.GetSize(); i++)
                {
                    flag = true;
                    if (dbmsLib::comparator(dbmsLib::Int32, (book.GetRow(i)).find("BookID")->second, dbmsLib::Equal,
                                            dbmsLib::GetValue((char *)buff_bookID, (string) "BookID", book.GetHeader())))
                    {
                        tmp_num = book.valueToString(book.GetRow(i), "SubjectID");
                        SubjectID = atoi(tmp_num.c_str());
                        for (size_t j = 0; j < abonements.GetSize(); j++)
                        {
                            if (dbmsLib::comparator(dbmsLib::Int32, (abonements.GetRow(j)).find("StudentID")->second, dbmsLib::Equal,
                                                    dbmsLib::GetValue((char *)buff, (string) "StudentID", abonements.GetHeader())))
                            {
                                bookIdTemp = abonements.valueToString(abonements.GetRow(j), "BookID");
                                for (size_t k = 0; k < book.GetSize(); k++)
                                {
                                    if (dbmsLib::comparator(dbmsLib::Int32, (book.GetRow(k)).find("BookID")->second, dbmsLib::Equal,
                                                            dbmsLib::GetValue((char *)bookIdTemp.c_str(), (string) "BookID", book.GetHeader())))
                                    {
                                        if (SubjectID == atoi(book.valueToString(book.GetRow(k), "SubjectID").c_str()))
                                        {

                                            if (dbmsLib::comparator(dbmsLib::Date, (abonements.GetRow(j)).find("InDate")->second, dbmsLib::Greater,
                                                                    dbmsLib::GetValue((char *)tmp_date.c_str(), (string) "InDate", abonements.GetHeader())))
                                            {
                                                flag = false;
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                            if (!flag)
                            {
                                break;
                            }
                        }
                        if (flag)
                        {
                            tmp_num = book.valueToString(book.GetRow(i), "Quantity");
                            quantity = atoi(tmp_num.c_str());
                            tmp_num = book.valueToString(book.GetRow(i), "Number");
                            number = atoi(tmp_num.c_str());
                            tmp_num = book.valueToString(book.GetRow(i), "BookID");
                            BookID = atoi(tmp_num.c_str());
                            if (quantity > number)
                            {
                                ++number;
                                _itoa_s(number, buff, 20, 10);
                                (book.GetRow(i)).find("Number")->second = dbmsLib::GetValue((char *)buff,
                                                                                            (string) "Number", book.GetHeader());

                                _itoa_s(StudentID, buff, 20, 10);
                                adress = dbmsLib::GetValue((char *)buff, (string) "StudentID", abonements.GetHeader());
                                row[(string) "StudentID"] = adress;
                                _itoa_s(BookID, buff, 20, 10);
                                adress = dbmsLib::GetValue((char *)buff, (string) "BookID", abonements.GetHeader());
                                row[(string) "BookID"] = adress;

                                adress = dbmsLib::GetValue((char *)tmp_date.c_str(), (string) "OutDate", abonements.GetHeader());
                                row[(string) "OutDate"] = adress;

                                dbmsLib::DBDate indate(tmp_date);
                                indate += 14;
                                adress = dbmsLib::GetValue((char *)(DateToStr(indate)).c_str(), (string) "InDate", abonements.GetHeader());
                                row[(string) "InDate"] = adress;

                                abonements.AddRow(row, abonements.GetSize());
                            }
                            else
                            {
                                cout << "Данной книги сейчас нет" << endl;
                            }
                        }
                        break;
                    }
                }
                if (!flag)
                {
                    cout << "У студента уже есть книга по этому предмету" << endl;
                }
            }
            else
            {
                cout << "Такого студента нет" << endl;
            }
        }
        break;
        case 5:
            return 0;
        default:
            break;
        }
    }
    return 0;
}