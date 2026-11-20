#include "util.h"
#include <iostream>
#include<cstring>
using namespace std;

void trimWhitespace(char* str) {
    if (str == nullptr) return;

    int start = 0;
    while (str[start] == ' ' || str[start] == '\t' || str[start] == '\r' || str[start] == '\n') {
        start++;
    }

    int i = 0;
    while (str[start + i] != '\0') {
        str[i] = str[start + i];
        i++;
    }
    str[i] = '\0';

    int end = i - 1;
    while (end >= 0 && (str[end] == ' ' || str[end] == '\t' || str[end] == '\r' || str[end] == '\n')) {
        str[end] = '\0';
        end--;
    }
}

int clampInt(int val, int minVal, int maxVal) {
    
    if(val > maxVal)
        return maxVal;
    if(val < minVal)
        return minVal;
    return val;
}

bool stringEquals(const char* a, const char* b){

    if(a == nullptr || b == nullptr)
        return false;
    
    if(strcmp(a , b) == 0){

        return true;

    }
    return false;

}

void printHeader(const char* title) {
    cout << "\n========================================\n";
    if (title != nullptr) {
        cout << title << '\n';
    }
    cout << "========================================\n";
}

void clearScreen(){

    cout<<"\n----------------------------------------------------------\n";
}