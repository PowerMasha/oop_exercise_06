#include <iostream>
#include <algorithm>
#include <map>
#include "square.h"
#include "conteiner/list.h"
#include "allocator/allocator.h"
int main() {
    setlocale(LC_ALL, "rus");
    size_t N;
    float S;
    char option = '0';
    containers::list<Square<int>, allocators::my_allocator<Square<int>, 500>> q;
    Square<int> kva{};
    while (option != 'q') {
        std::cout << "выберите опцию (m for man, q to quit)" << std::endl;
        std:: cin >> option;
        switch (option) {
            case 'q':
                break;
            case 'm': {
                std::cout << "1. Добавить фигуру \n"
                          << "2. Удалить фигуру \n"
                          << "3. Вывести фигуру по индексу\n"
                          << "o. Вывести все фигуры\n"
                          << "a. Вывести кол-во фигур чья площадь меньше чем ...\n";
                break;
            }


            case '1': {
                std::cout << "позиция для вставки: ";
                std::cin >> N;
                std::cout << "введите квадрат: \n";
                kva = Square<int>(std::cin);
                try {
                    kva.Check();
                } catch (std::logic_error &err) {
                    std::cout << err.what() << std::endl;
                    break;
                }
                try {
                    q.insert_by_number(N, kva);
                } catch (std::logic_error &err) {
                    q.delete_by_number(N);
                    std::cout << err.what() << std::endl;
                    break;
                }

                break;
            }
            case '2': {
                std::cout << "позиция для удаления: ";
                std::cin >> N;
                if (N==0){
                    q.pop_front();

                }else {
                    if (N == (q.length() - 1)) {
                        q.pop_back();
                    } else {
                        try {
                            q.delete_by_number(N);
                        } catch (std::logic_error &err) {
                            std::cout << err.what() << std::endl;
                            break;
                        }
                    }
                }
                break;
            }
            case '3': {
                std::cout << "введите индекс элемента: ";
                std::cin >> N;
                q[N].Printout(std::cout);
                break;
            }
            case 'o': {
                std::for_each(q.begin(), q.end(), [](Square<int> &X) { X.Printout(std::cout); });
                break;
            }
            case 'a': {
                std::cout << "площадь для сравнения: ";
                std::cin >> S;
                std::cout << "количестов элементов с площадью меньше чем " << S << " :"
                          << std::count_if(q.begin(), q.end(), [=](Square<int> &X) { return X.Area() < S; })
                          << std::endl;
                break;
            }
            default:
                break;
        }
    }
    return 0;
}
