#ifndef FUNCTION_H
#define FUNCTION_H
#include <initializer_list>
#include <string>
class Five{

private:
    unsigned char* data;
    size_t size;

public:
    Five();
    Five(const size_t  &n, unsigned char t = 0);
    Five(const std::initializer_list< unsigned char > &t);
    Five(const std::string &t);
    Five(const Five& other);
    Five(Five&& other) noexcept;
    virtual ~Five() noexcept;
    Five operator+(const Five& other) const;
    Five operator-(const Five& other) const;
    bool operator==(const Five& other) const;
    bool operator!=(const Five& other) const;
    bool operator<(const Five& other) const;
    bool operator>(const Five& other) const;
    size_t getSize() const { return size; }
    const unsigned char* getData() const { return data; }
};

#endif