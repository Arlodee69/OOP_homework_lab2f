#include "../include/function.h"
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

    data = new unsigned char[size];

    for (size_t i = 0; i < size; i++){
        data[i] = t;
    }
}

Five::Five(const std::initializer_list< unsigned char > &t){
    size = t.size();

    data = new unsigned char[size];

    size_t i = 0;
    for (unsigned char value : t){
        if (value > 4){
            delete[] data;
            throw std::invalid_argument("Digit must be between 0 and 4");
            }
        data[size - 1 - i] = value;
        i++;
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
        data[size - 1 - i] = c - '0';
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

Five Five::add(const Five& other) const{
    size_t maxSize = std::max(size, other.size);
    size_t resultSize = maxSize + 1;
    unsigned char* resultData = new unsigned char[resultSize]();
    int carry = 0;
    for(size_t k = 0; k < resultSize; ++k) {
        int sum = carry;
        if(k < size) {
            sum += data[k];
        }
        if(k < other.size) {
            sum += other.data[k];
        }

        carry = sum / 5;
        resultData[k] = sum % 5;
    }

    size_t finalSize = resultSize;
    while(finalSize > 1 && resultData[finalSize - 1] == 0) {
        finalSize--;
    }

    Five result;
    result.size = finalSize;
    result.data = new unsigned char[finalSize];
    for(size_t i = 0; i < finalSize; ++i) {
        result.data[i] = resultData[i];
    }

    delete[] resultData;
    return result;
}

Five Five::copy() const{
    return(Five(*this));
}

Five Five::subtract(const Five& other) const{
    if (lessthan(other)){
        throw std::underflow_error("Result would be negative");
    }
    size_t maxSize = std::max(size, other.size);
    unsigned char* resultData = new unsigned char[maxSize]();
    int borrow = 0;


    for(size_t k = 0; k < maxSize; ++k) {
        int top, bottom;

        if(k < size) {
            top = data[k];
        } else {
            top = 0;
        }
        top -= borrow;
        if(k < other.size) {
            bottom = other.data[k];
        } else {
            bottom = 0;
        }
        if(top < bottom) {
            top += 5;
            borrow = 1;
        } else {
            borrow = 0;
        }

        resultData[k] = top - bottom;
    }
    size_t finalSize = maxSize;
    while(finalSize > 1 && resultData[finalSize - 1] == 0) {
        finalSize--;
    }

    Five result;
    result.size = finalSize;
    result.data = new unsigned char[finalSize];
    for(size_t i = 0; i < finalSize; ++i) {
        result.data[i] = resultData[i];
    }

    delete[] resultData;
    return result;
    }

bool Five::equals(const Five& other) const {
    if (size != other.size) return false;
    for (size_t i = 0; i < size; ++i) {
        if (data[i] != other.data[i]) return false;
    }
    return true;
}

bool Five::lessthan(const Five& other) const {
    if (size != other.size) return size < other.size;
    for (size_t i = size; i > 0; --i) {
        size_t idx = i - 1;
        if (data[idx] != other.data[idx]) {
            return data[idx] < other.data[idx];
        }
    }
    return false;
}

bool Five::greaterthan(const Five& other) const {
    return !(lessthan(other)) && !equals(other);
}

int main(){
    Five a("1234");
    Five b(a);
    std::cout<<&a<<std::endl;
    std::cout<<&b<<std::endl;
}