#include <gtest/gtest.h>
#include <stdexcept>
#include <string>
#include <sstream>
#include "../include/function.h"

// --- Тесты конструкторов ---

TEST(FiveTest, DefaultConstructor) {
    Five num;
    EXPECT_EQ(num.getSize(), 0);
    EXPECT_EQ(num.getData(), nullptr);
}

TEST(FiveTest, SizeValueConstructor) {
    Five num(3, 2);
    EXPECT_EQ(num.getSize(), 3);
    const unsigned char* data = num.getData();
    // num(3, 2) -> "222" -> data[0]=2, data[1]=2, data[2]=2
    for (size_t i = 0; i < num.getSize(); ++i) {
        EXPECT_EQ(data[i], 2);
    }
}

TEST(FiveTest, SizeValueConstructorInvalidDigit) {
    EXPECT_THROW(Five(3, 5), std::invalid_argument);
    EXPECT_THROW(Five(3, 10), std::invalid_argument);
}

TEST(FiveTest, InitializerListConstructor) {
    // Five num = {1, 2, 3, 4}; // Представляет число 1234₅
    // Внутреннее представление: индекс 0 - младшая цифра
    // data[0]=4, data[1]=3, data[2]=2, data[3]=1
    Five num = {1, 2, 3, 4};
    EXPECT_EQ(num.getSize(), 4);

    const unsigned char* data = num.getData();
    EXPECT_EQ(data[0], 4); // младшая цифра
    EXPECT_EQ(data[1], 3);
    EXPECT_EQ(data[2], 2);
    EXPECT_EQ(data[3], 1); // старшая цифра
}

TEST(FiveTest, InitializerListConstructorInvalidDigit) {
    EXPECT_THROW(Five({1, 5, 3}), std::invalid_argument);
    EXPECT_THROW(Five({1, 2, 6}), std::invalid_argument);
}

TEST(FiveTest, StringConstructor) {
    // Five num("1234"); // Представляет число 1234₅
    // Внутреннее представление: индекс 0 - младшая цифра
    // data[0]=4, data[1]=3, data[2]=2, data[3]=1
    Five num("1234");
    EXPECT_EQ(num.getSize(), 4);

    const unsigned char* data = num.getData();
    EXPECT_EQ(data[0], 4); // младшая цифра
    EXPECT_EQ(data[1], 3);
    EXPECT_EQ(data[2], 2);
    EXPECT_EQ(data[3], 1); // старшая цифра
}

TEST(FiveTest, StringConstructorEmpty) {
    Five num("");
    EXPECT_EQ(num.getSize(), 0);
    EXPECT_EQ(num.getData(), nullptr);
}

TEST(FiveTest, StringConstructorInvalidCharacters) {
    EXPECT_THROW(Five("567"), std::invalid_argument);
    EXPECT_THROW(Five("12a4"), std::invalid_argument);
    EXPECT_THROW(Five("1-23"), std::invalid_argument);
    EXPECT_THROW(Five("12 3"), std::invalid_argument); // Пробел
    EXPECT_THROW(Five("12A3"), std::invalid_argument); // Заглавная буква
}

TEST(FiveTest, CopyConstructor) {
    Five original("123"); // Создаёт data={3, 2, 1}
    Five copy(original);
    EXPECT_EQ(copy.getSize(), original.getSize());
    EXPECT_TRUE(copy.equals(original));
    // Проверяем, что это разные объекты
    const unsigned char* orig_data = original.getData();
    const unsigned char* copy_data = copy.getData();
    EXPECT_NE(orig_data, copy_data); // Указатели должны быть разными
    EXPECT_EQ(orig_data[0], copy_data[0]);
    EXPECT_EQ(orig_data[1], copy_data[1]);
    EXPECT_EQ(orig_data[2], copy_data[2]);
}

TEST(FiveTest, MoveConstructor) {
    Five original("123"); // Создаёт data={3, 2, 1}
    Five moved(std::move(original));
    EXPECT_EQ(moved.getSize(), 3);
    // Оригинал должен быть в валидном, но неопределенном состоянии
    EXPECT_EQ(original.getSize(), 0);
    EXPECT_EQ(original.getData(), nullptr);

    const unsigned char* data = moved.getData();
    EXPECT_EQ(data[0], 3); // младшая цифра
    EXPECT_EQ(data[1], 2);
    EXPECT_EQ(data[2], 1); // старшая цифра
}

// --- Тесты арифметических операций ---

// Сложение: 123₅ (38₁₀) + 234₅ (69₁₀) = 412₅ (107₁₀)
// 123₅ (data={3, 2, 1}) + 234₅ (data={4, 3, 2}) = 412₅ (data={2, 1, 4})
TEST(FiveTest, AdditionSameLength) {
    Five a("123");  // data={3, 2, 1}
    Five b("234");  // data={4, 3, 2}
    Five result = a.add(b);  // 38 + 69 = 107₁₀ = 412₅ -> data={2, 1, 4}

    Five expected("412"); // data={2, 1, 4}
    EXPECT_TRUE(result.equals(expected));
}

// Сложение: 1234₅ (194₁₀) + 12₅ (7₁₀) = 1301₅ (201₁₀)
// 1234₅ (data={4, 3, 2, 1}) + 12₅ (data={2, 1}) = 1301₅ (data={1, 0, 3, 1})
TEST(FiveTest, AdditionDifferentLength) {
    Five a("1234"); // data={4, 3, 2, 1}
    Five b("12");   // data={2, 1}
    Five result = a.add(b);  // 194 + 7 = 201₁₀ = 1301₅ -> data={1, 0, 3, 1}

    Five expected("1301"); // data={1, 0, 3, 1}
    EXPECT_TRUE(result.equals(expected));
}

// Сложение: 44₅ (24₁₀) + 11₅ (6₁₀) = 110₅ (30₁₀)
// 44₅ (data={4, 4}) + 11₅ (data={1, 1}) = 110₅ (data={0, 1, 1})
TEST(FiveTest, AdditionWithCarry) {
    Five a("44");   // data={4, 4}
    Five b("11");   // data={1, 1}
    Five result = a.add(b);  // 24 + 6 = 30₁₀ = 110₅ -> data={0, 1, 1}

    Five expected("110"); // data={0, 1, 1}
    EXPECT_TRUE(result.equals(expected));
}

// Сложение с нулём
TEST(FiveTest, AdditionZero) {
    Five a("123"); // data={3, 2, 1}
    Five zero("0"); // data={0}
    Five result = a.add(zero);

    EXPECT_TRUE(result.equals(a));
}

// Сложение с нулем слева
TEST(FiveTest, AdditionZeroLeft) {
    Five a("123"); // data={3, 2, 1}
    Five zero("0"); // data={0}
    Five result = zero.add(a);

    EXPECT_TRUE(result.equals(a));
}

// Сложение с нулем справа
TEST(FiveTest, AdditionZeroRight) {
    Five a("123"); // data={3, 2, 1}
    Five zero("0"); // data={0}
    Five result = a.add(zero);

    EXPECT_TRUE(result.equals(a));
}

// Сложение с нулем слева и справа
TEST(FiveTest, AdditionZeroBoth) {
    Five zero1("0"); // data={0}
    Five zero2("0"); // data={0}
    Five result = zero1.add(zero2);

    Five expected("0"); // data={0}
    EXPECT_TRUE(result.equals(expected));
}

// Вычитание: 234₅ (69₁₀) - 123₅ (38₁₀) = 111₅ (31₁₀)
// 234₅ (data={4, 3, 2}) - 123₅ (data={3, 2, 1}) = 111₅ (data={1, 1, 1})
TEST(FiveTest, SubtractionSameLength) {
    Five a("234");  // data={4, 3, 2}
    Five b("123");  // data={3, 2, 1}
    Five result = a.subtract(b);  // 69 - 38 = 31₁₀ = 111₅ -> data={1, 1, 1}

    Five expected("111"); // data={1, 1, 1}
    EXPECT_TRUE(result.equals(expected));
}

// Вычитание: 12₅ (7₁₀) - 123₅ (38₁₀) -> ошибка
TEST(FiveTest, SubtractionNegativeResult) {
    Five a("12");   // data={2, 1} -> 7₁₀
    Five b("123");  // data={3, 2, 1} -> 38₁₀

    EXPECT_THROW(a.subtract(b), std::underflow_error);
}

// Вычитание: 123₅ (38₁₀) - 12₅ (7₁₀) = 111₅ (31₁₀)
// 123₅ (data={3, 2, 1}) - 12₅ (data={2, 1}) = 111₅ (data={1, 1, 1})
TEST(FiveTest, SubtractionDifferentLength) {
    Five a("123");  // data={3, 2, 1}
    Five b("12");   // data={2, 1}
    Five result = a.subtract(b);  // 38 - 7 = 31₁₀ = 111₅ -> data={1, 1, 1}

    Five expected("111"); // data={1, 1, 1}
    EXPECT_TRUE(result.equals(expected));
}

// Вычитание: 123₅ (38₁₀) - 123₅ (38₁₀) = 0₅ (0₁₀)
// 123₅ (data={3, 2, 1}) - 123₅ (data={3, 2, 1}) = 0₅ (data={0})
TEST(FiveTest, SubtractionSameNumber) {
    Five a("123");  // data={3, 2, 1}
    Five b("123");  // data={3, 2, 1}
    Five result = a.subtract(b);  // 38 - 38 = 0₁₀ = 0₅ -> data={0}

    Five expected("0"); // data={0}
    EXPECT_TRUE(result.equals(expected));
}

// --- Тесты операторов сравнения ---

TEST(FiveTest, Equality) {
    Five a("123"); // data={3, 2, 1}
    Five b("123"); // data={3, 2, 1}
    Five c("124"); // data={4, 2, 1}

    EXPECT_TRUE(a.equals(b));
    EXPECT_FALSE(a.equals(c));
}

TEST(FiveTest, EqualityDifferentSizes) {
    Five a("12");  // data={2, 1}
    Five b("012"); // data={2, 1, 0}
    EXPECT_FALSE(a.equals(b)); // Размеры разные: 2 vs 3

    Five d("00"); // data={0, 0}
    Five e("0");  // data={0}
    EXPECT_FALSE(d.equals(e)); // Размеры разные: 2 vs 1
}

// Сравнение: 12₅ (7₁₀) < 123₅ (38₁₀)
// 12₅ (data={2, 1}) vs 123₅ (data={3, 2, 1})
TEST(FiveTest, LessThan) {
    Five a("12");   // data={2, 1} -> 7₁₀
    Five b("123");  // data={3, 2, 1} -> 38₁₀

    EXPECT_TRUE(a.lessthan(b));  // 7 < 38
    EXPECT_FALSE(b.lessthan(a)); // 38 не < 7
}

// Дополнительный тест LessThan: одинаковая длина, разные цифры
// 12₅ (7₁₀) vs 13₅ (8₁₀)
// 12₅ (data={2, 1}) vs 13₅ (data={3, 1})
TEST(FiveTest, LessThanSameLength) {
    Five a("12");   // data={2, 1} -> 7₁₀
    Five b("13");   // data={3, 1} -> 8₁₀

    EXPECT_TRUE(a.lessthan(b));  // 7 < 8
    EXPECT_FALSE(b.lessthan(a)); // 8 не < 7
}

// Дополнительный тест LessThan: разная длина
// 12₅ (7₁₀) vs 123₅ (38₁₀)
// 12₅ (data={2, 1}) vs 123₅ (data={3, 2, 1})
TEST(FiveTest, LessThanDifferentLength) {
    Five a("12");   // data={2, 1} -> 2 цифры
    Five b("123");  // data={3, 2, 1} -> 3 цифры

    EXPECT_TRUE(a.lessthan(b));  // 2 < 3 (по длине)
    EXPECT_FALSE(b.lessthan(a));
}

// Дополнительный тест LessThan: одинаковая длина, первые цифры равны
// 123₅ (38₁₀) vs 124₅ (39₁₀)
// 123₅ (data={3, 2, 1}) vs 124₅ (data={4, 2, 1})
TEST(FiveTest, LessThanSameLengthCompareDigits) {
    Five a("123");   // data={3, 2, 1}
    Five b("124");   // data={4, 2, 1}

    EXPECT_TRUE(a.lessthan(b));  // 38 < 39
    EXPECT_FALSE(b.lessthan(a)); // 39 не < 38
}

// Тест GreaterThan: основывается на !lessthan(other)
// a > b  означает !(a < b) или (b < a)
TEST(FiveTest, GreaterThan) {
    Five a("123");  // data={3, 2, 1} -> 38₁₀
    Five b("12");   // data={2, 1} -> 7₁₀

    // greaterthan(a, b) -> !(a < b) -> !(38 < 7) -> true
    // greaterthan(b, a) -> !(b < a) -> !(7 < 38) -> false
    EXPECT_TRUE(a.greaterthan(b)); // 38 > 7
    EXPECT_FALSE(b.greaterthan(a)); // 7 не > 38

    Five c("123"); // data={3, 2, 1} -> 38₁₀
    EXPECT_FALSE(a.greaterthan(c)); // 38 не > 38
}

// Тест GreaterThanSameLength
TEST(FiveTest, GreaterThanSameLength) {
    Five a("13");   // data={3, 1} -> 8₁₀
    Five b("12");   // data={2, 1} -> 7₁₀

    EXPECT_TRUE(a.greaterthan(b)); // 8 > 7
    EXPECT_FALSE(b.greaterthan(a)); // 7 не > 8
}

// Тест GreaterThanDifferentLength
TEST(FiveTest, GreaterThanDifferentLength) {
    Five a("123");  // data={3, 2, 1} -> 3 цифры
    Five b("12");   // data={2, 1} -> 2 цифры

    EXPECT_TRUE(a.greaterthan(b)); // 3 > 2 (по длине)
    EXPECT_FALSE(b.greaterthan(a)); // 2 не > 3
}

// --- Тесты операторов ---
// Добавляем тесты для операторов, если они реализованы в function.h
// Текущий код не показывает операторов, кроме методов.
// Если добавить операторы в function.h, тесты будут выглядеть так:

/*
// Пример теста для operator+
TEST(FiveTest, OperatorAddition) {
    Five a("123");  // 38₁₀
    Five b("234");  // 69₁₀
    Five result = a + b; // Предполагаем, что operator+ вызывает add
    Five expected("412"); // 107₁₀
    EXPECT_TRUE(result == expected);
}

// Пример теста для operator-
TEST(FiveTest, OperatorSubtraction) {
    Five a("234");  // 69₁₀
    Five b("123");  // 38₁₀
    Five result = a - b; // Предполагаем, что operator- вызывает subtract
    Five expected("111"); // 31₁₀
    EXPECT_TRUE(result == expected);
}

// Пример теста для operator==
TEST(FiveTest, OperatorEquality) {
    Five a("123");
    Five b("123");
    EXPECT_TRUE(a == b);
    Five c("124");
    EXPECT_FALSE(a == c);
}

// Пример теста для operator<
TEST(FiveTest, OperatorLessThan) {
    Five a("12");
    Five b("123");
    EXPECT_TRUE(a < b);
    EXPECT_FALSE(b < a);
}

// Пример теста для operator>
TEST(FiveTest, OperatorGreaterThan) {
    Five a("123");
    Five b("12");
    EXPECT_TRUE(a > b);
    EXPECT_FALSE(b > a);
}

// Пример теста для operator!=
TEST(FiveTest, OperatorNotEquals) {
    Five a("123");
    Five b("124");
    EXPECT_TRUE(a != b);
    Five c("123");
    EXPECT_FALSE(a != c);
}

// Пример теста для operator<=
TEST(FiveTest, OperatorLessThanOrEqual) {
    Five a("123");
    Five b("123");
    Five c("124");
    EXPECT_TRUE(a <= b); // равно
    EXPECT_TRUE(a <= c); // меньше
    EXPECT_FALSE(c <= a); // c > a
}

// Пример теста для operator>=
TEST(FiveTest, OperatorGreaterThanOrEqual) {
    Five a("123");
    Five b("123");
    Five c("122");
    EXPECT_TRUE(a >= b); // равно
    EXPECT_TRUE(a >= c); // больше
    EXPECT_FALSE(c >= a); // c < a
}
*/

// --- Запуск тестов ---

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}