#include "util.h"
#include <iostream>
using namespace std;

int main() {
    char sample[] = "123   ";
    trimWhitespace(sample);

    char a[] = "123";

    cout<<stringEquals(a , sample);


    
    return 0;
}