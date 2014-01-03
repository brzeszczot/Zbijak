/*
 * Alg.hpp
 *
 *  Created on: Nov 22, 2012
 *      Author: brzeszczot
 */

#ifndef ALG_HPP_
#define ALG_HPP_

#include <cmath>
#include <algorithm>
#include <vector>
#include "Adds.hpp"

using namespace std;

class Algorithms
{
    public:
        Algorithms();
        ~Algorithms();
        // oblicz dlugsc osdcinka (punkt 1, punkt 2)
        static float SectionLength(Adds::vertex v1, Adds::vertex v2);
        // sprawdz czy p1 widzi p2 gdzie v1 to lewy gorny rog a v2 to szer  i wysokosc przeszkody
        static bool ObjectVisible(Adds::vertex p1, Adds::vertex p2, Adds::vertex v1, Adds::vertex v2);
        // wyznacz punkty odcinka (punkt 1, punkt 2, co ile punktow (1), vector punktow, opcjonalnie do ilu ma liczyc)
        static Adds::vertex SectionCalc(Adds::vertex p1, Adds::vertex p2, int bytesPerLine, vector<Adds::vertex> *v, int deep = -1);
};

#endif /* ALG_HPP_ */
