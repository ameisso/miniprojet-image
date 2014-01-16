#include "fonctions.h"

#include <iostream>
#include <sstream>

using namespace std;

/*
 * Fonction qui convertie un int en string.
 **/
string intToString(int val)
{
    stringstream ss;
    ss << val;
    return ss.str();
}
