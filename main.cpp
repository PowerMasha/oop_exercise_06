#include <iostream>
#include <algorithm>
#include "square.h"
#include <map>
#include "conteiner/stack.h"
#include "allocator/allocator.h"

int main() {
size_t N;
float S;
char option = '0';
containers::stack<Square<int>, allocators::my_allocator<Square<int>, 800 >> q;
Square<int> kva{};
while (option != 'q') {
std::cout << "choose option (m - man, q -quite)" << std::endl;
std::cin >> option;
switch (option) {
            case 'q':
                break;
            case 'm':
                std::cout << "1) добавить новый элемент в стэк\n"
                          << "2) вставить элемент на позицию\n"
                          << "3) (pop)удаление верхнего элемента\n"
                          << "4) (top) значение вернего элемента\n"
                          << "5) удалить элемент с позиции\n"
                          << "6) напечатать стэк\n"
                          << "7) количесто элементов с площадью меньше чем \n" 
			  << "8) map\n"
			  << std::endl;
                break;
            case '1': {
                std::cout << "введите вершины квадрата: " << std::endl;
                kva = Square<int>(std::cin);
                try{
                    kva.Check();
                }catch(std::logic_error& err){
                    std::cout << err.what() << std::endl;
                    break;
                }
                q.push(kva);
                break;
            }
            case '2': {
                std::cout << "позиция для вставки: ";
                std::cin >> N;
                std::cout << "введите квадрат: ";
                kva =Square<int>(std::cin);
                q.insert_by_number(N+1, kva);
                break;
            }
            case '3': {
                q.pop();
                break;
            }
            case '4': {
                q.top().Printout(std::cout);
                break;
            }
            case '5': {
                std::cout << "позиция для удаления: ";
                std::cin >> N;
                q.delete_by_number(N+1);
                break;
            }
            case '6': {
                std::for_each(q.begin(), q.end(), [](Square<int> &X) { X.Printout(std::cout); });
                break;
            }
            case '7': {
                std::cout << "площадь для сравнения: ";
                std::cin >> S;
                std::cout <<"количестов элементов с площадью меньше чем" << S << " :" << std::count_if(q.begin(), q.end(), [=](Square<int>& X){return X.Area() < S;}) << std::endl;
                break;
            }
		case '8': {
                std::map<int, int, std::less<>, allocators::my_allocator<std::pair<const int, int>, 100>> mp;
                for(int i = 0; i < 2; ++i){
                    mp[i] = i * i;
                }
                std::for_each(mp.begin(), mp.end(), [](std::pair<int, int> X) { std::cout << X.first << ' ' << X.second << ",  "; });
                std::cout << std::endl;
                for(int i = 2; i < 10; ++i){
                    mp.erase(i - 2);
                    mp[i] = i * i;
                }
                std::for_each(mp.begin(), mp.end(), [](std::pair<int, int> X) { std::cout << X.first << ' ' << X.second << ",  "; });
                std::cout << std::endl;
                break;
            }
            default:
                std::cout << "нет такой опции. Попробуйте m" << std::endl;
                break;
        }
    }
    return 0;
}
