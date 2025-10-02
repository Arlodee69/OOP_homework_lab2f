#include <gtest/gtest.h>
#include <stdexcept>
#include "../include/function.h"

// Тесты конструкторов
TEST(FiveTest, DefaultConstructor) {
    Five num;
    EXPECT_EQ(num.getSize(), 0);
    EXPECT_EQ(num.getData(), nullptr);
}

TEST(FiveTest, SizeValueConstructor) {
    Five num(3, 2);
    EXPECT_EQ(num.getSize(), 3);
    // Проверяем что все элементы равны 2
    const unsigned char* data = num.getData();
    for (size_t i = 0; i < num.getSize(); ++i) {
        EXPECT_EQ(data[i], 2);
    }
}

TEST(FiveTest, SizeValueConstructorInvalidDigit) {
    EXPECT_THROW(Five(3, 5), std::invalid_argument);
    EXPECT_THROW(Five(3, 10), std::invalid_argument);
}

TEST(FiveTest, InitializerListConstructor) {
    Five num = {1, 2, 3, 4};
    EXPECT_EQ(num.getSize(), 4);
    
    const unsigned char* data = num.getData();
    EXPECT_EQ(data[0], 1);
    EXPECT_EQ(data[1], 2);
    EXPECT_EQ(data[2], 3);
    EXPECT_EQ(data[3], 4);
}

TEST(FiveTest, InitializerListConstructorInvalidDigit) {
    EXPECT_THROW(Five({1, 5, 3}), std::invalid_argument);
    EXPECT_THROW(Five({1, 2, 6}), std::invalid_argument);
}

TEST(FiveTest, StringConstructor) {
    Five num("1234");
    EXPECT_EQ(num.getSize(), 4);
    
    const unsigned char* data = num.getData();
    EXPECT_EQ(data[0], 1);
    EXPECT_EQ(data[1], 2);
    EXPECT_EQ(data[2], 3);
    EXPECT_EQ(data[3], 4);
}

TEST(FiveTest, StringConstructorInvalidCharacters) {
    EXPECT_THROW(Five("567"), std::invalid_argument);
    EXPECT_THROW(Five("12a4"), std::invalid_argument);
    EXPECT_THROW(Five("1-23"), std::invalid_argument);
}

TEST(FiveTest, CopyConstructor) {
    Five original("123");
    Five copy(original);
    EXPECT_EQ(copy.getSize(), original.getSize());
    EXPECT_TRUE(copy == original);
}

TEST(FiveTest, MoveConstructor) {
    Five original("123");
    Five moved(std::move(original));
    EXPECT_EQ(moved.getSize(), 3);
    EXPECT_EQ(original.getSize(), 0);
    EXPECT_EQ(original.getData(), nullptr);
}

TEST(FiveTest, AdditionSameLength) {
    Five a("123");  // 38₁₀
    Five b("234");  // 69₁₀
    Five result = a + b;  // 107₁₀ = 412₅
    
    Five expected("412");
    EXPECT_TRUE(result == expected);
}

TEST(FiveTest, AdditionDifferentLength) {
    Five a("1234");  // 194₁₀
    Five b("12");    // 7₁₀
    Five result = a + b;  // 201₁₀ = 1301₅
    
    Five expected("1301");
    EXPECT_TRUE(result == expected);
}

TEST(FiveTest, AdditionWithCarry) {
    Five a("44");   // 24₁₀
    Five b("11");   // 6₁₀
    Five result = a + b;  // 30₁₀ = 110₅
    
    Five expected("110");
    EXPECT_TRUE(result == expected);
}

TEST(FiveTest, AdditionZero) {
    Five a("123");
    Five zero("0");
    Five result = a + zero;
    
    EXPECT_TRUE(result == a);
}

// ... остальные тесты без изменений ...

TEST(FiveTest, SubtractionSameLength) {
    Five a("234");  // 69₁₀
    Five b("123");  // 38₁₀
    Five result = a - b;  // 31₁₀ = 111₅
    
    Five expected("111");
    EXPECT_TRUE(result == expected);
}

TEST(FiveTest, SubtractionNegativeResult) {
    Five a("12");   // 7₁₀
    Five b("123");  // 38₁₀
    
    EXPECT_THROW(a - b, std::underflow_error);
}

TEST(FiveTest, Equality) {
    Five a("123");
    Five b("123");
    Five c("124");
    
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a == c);
}

TEST(FiveTest, LessThan) {
    Five a("12");   // 7₁₀
    Five b("123");  // 38₁₀
    
    EXPECT_TRUE(a < b);
    EXPECT_FALSE(b < a);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}