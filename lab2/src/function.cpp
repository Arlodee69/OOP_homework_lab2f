#include "function.h"
#include <iostream>
#include <cstdio>
#include <stdexcept>

Five::Five(){
    data = nullptr;
    size = 0;   
}

Five::Five(const size_t& n, unsigned char t){
    if (t > 4) throw std::invalid_argument("Digit must be between 0 and 4");
    size = n;
    try {data = new unsigned char[size];
    } catch (const std::bad_alloc& e){
        printf("Allocation Error");
    }
    for (size_t i = 0; i < size; i++){
        data[i] = t;
    }
}

Five::Five(const std::initializer_list< unsigned char > &t){
    size = t.size();
    data = new unsigned char[size];
    size_t i = 0;
    for (unsigned char value : t){
        if (value > 4) throw std::invalid_argument("Digit must be between 0 and 4");
        data[i++] = value;
    }
}

Five::Five(const std::string &t){
    if (t.empty()){
        size = 0;
        data = nullptr;
        return;
    }
    size = t.length();
    data = new unsigned char[size];
    for (size_t i = 0; i< size; i++){
        char c = t[i];
        if (c < '0' || c > '4'){
            delete[] data;
            throw std::invalid_argument("String must contain only digits 0-4");
        }
        data[i] = c - '0';
    }
}

Five::Five(const Five& other){
    size = other.size;
    data = new unsigned char[size];
    for (size_t i = 0; i < size; i++){
        data[i] = other.data[i];
    }
}
Five::Five(Five&& other) noexcept{
    size = other.size;
    data = other.data;
    other.data = nullptr;
    other.size = 0;
}

Five::~Five() noexcept{
    delete[] data;
    data = nullptr;
}

Five Five::operator+(const Five& other) const{
    size_t maxSize = std::max(size, other.size);
    size_t resultSize = maxSize + 1;
    unsigned char* resultData = new unsigned char[resultSize]();
    int carry = 0;
    int i = size - 1;
    int j = other.size - 1;
    int k = resultSize - 1;

    while (i >= 0 || j >= 0 || carry > 0){
        int sum = carry;
        
        if (i >= 0){
            sum += data[i];
            i--;
        }
        
        if (j >= 0){
            sum += other.data[j];
            j--;
        }
        carry = sum / 5;
        resultData[k] = sum % 5;
        k--;
    }
    size_t startIndex = 0;
    while (startIndex < resultSize - 1 && resultData[startIndex] == 0){
        startIndex++;
    }
    Five result;
    result.size = resultSize - startIndex;
    result.data = new unsigned char[result.size];
    
    for (size_t i = 0; i < result.size; i++){
        result.data[i] = resultData[startIndex + i];
    }
    delete[] resultData;
    return result;
}

Five Five::operator-(const Five& other) const{
    if (*this < other){
        throw std::underflow_error("Result would be negative");
    }
    size_t maxSize = std::max(size, other.size);
    unsigned char* resultData = new unsigned char[maxSize]();
    
    int borrow = 0;
    int i = size - 1;
    int j = other.size - 1;
    int k = maxSize - 1;
    
    while (i >= 0 || j >= 0) {
        int top, bottom;
        if (i >= 0){
            top = data[i];
        }
        else{
            top = 0;
        }
        if (j >= 0){
            bottom = other.data[j];
        }
        else{
            bottom = 0;
        }
        top -= borrow;
        
        if (top < bottom) {
            top += 5;
            borrow = 1;
        } else {
            borrow = 0;
        }
        
        resultData[k] = top - bottom;
        i--;
        j--;
        k--;
    }
    size_t startIndex = 0;
    while (startIndex < maxSize - 1 && resultData[startIndex] == 0) {
        startIndex++;
    }
    
    Five result;
    result.size = maxSize - startIndex;
    result.data = new unsigned char[result.size];
    for (size_t i = 0; i < result.size; i++){
        result.data[i] = resultData[startIndex + i];
    }
    delete[] resultData;
    return result;
}

bool Five::operator==(const Five& other) const {
    if (size != other.size) return false;
    for (size_t i = 0; i < size; ++i) {
        if (data[i] != other.data[i]) return false;
    }
    return true;
}

bool Five::operator!=(const Five& other) const {
    return !(*this == other);
}

bool Five::operator<(const Five& other) const {
    if (size != other.size) return size < other.size;
    for (size_t i = 0; i < size; ++i) {
        if (data[i] != other.data[i]) return data[i] < other.data[i];
    }
    return false;
}

bool Five::operator>(const Five& other) const {
    return other < *this;
}