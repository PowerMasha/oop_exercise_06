#ifndef SQUARE
#define SQUARE

#include "vertex.h"

template <class T>
class Square {
public:
    vertex<T> points[4];

    explicit Square<T>(std::istream& is) {
        for (auto & point : points) {
            is >> point;
        }
    }
    Square<T>() = default;

    double Area() const {
        double res = 0;
        for (size_t i = 0; i < 3; i++) {
            res += (points[i].x * points[i+1].y) - (points[i+1].x * points[i].y);
        }
        res = res + (points[3].x * points[0].y) - (points[0].x * points[3].y);
        return std::abs(res)/ 2;
    }

    void Printout(std::ostream& os) {
        for (int i = 0; i < 4; ++i) {
            os << this->points[i];
            if (i != 3) {
                os << ", ";
            }
        }
        os << std::endl;
    }
    void Check() {
        double a, b, c, d, d1, d2, ABC, BCD, CDA, DAB;
        a = sqrt((points[2].x- points[1].x) * (points[2].x - points[1].x) + (points[2].y - points[1].y) * (points[2].y - points[1].y));
        b = sqrt((points[3].x- points[2].x) * (points[3].x - points[2].x) + (points[3].y - points[2].y) * (points[3].y - points[2].y));
        c = sqrt((points[3].x- points[4].x) * (points[3].x - points[4].x) + (points[3].y - points[4].y) * (points[3].y - points[4].y));
        d = sqrt((points[4].x- points[1].x) * (points[4].x - points[1].x) + (points[4].y - points[1].y) * (points[4].y - points[1].y));
        d1 = sqrt((points[2].x- points[4].x) * (points[2].x - points[4].x) + (points[2].y - points[4].y) * (points[2].y - points[4].y));
        d2 = sqrt((points[3].x- points[1].x) * (points[3].x - points[1].x) + (points[3].y - points[1].y) * (points[3].y - points[1].y));
        ABC = (a * a + b * b - d2 * d2) / 2 * a * b;
        BCD = (b * b + c * c - d1 * d1) / 2 * b * c;
        CDA = (d * d + c * c - d2 * d2) / 2 * d * c;
        DAB = (a * a + d * d - d1 * d1) / 2 * a * d;
        if(ABC != BCD || ABC != CDA || ABC != DAB || a!=b || a!=c || a!=d )
            throw std::logic_error("Это не квадрат!");
    }

    void operator<< (std::ostream& os) {
        for (int i = 0; i < 4; ++i) {
            os << this->points[i];
            if (i != 3) {
                os << ", ";
            }
        }
    }
};

#endif
