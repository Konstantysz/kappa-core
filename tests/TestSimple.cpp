#include <gtest/gtest.h>
#include <string>

// Minimal test to isolate the problem - NO kappa headers at all

TEST(SimpleTest, StringOnStack)
{
    std::string s = "Hello";
    EXPECT_EQ(s, "Hello");
}

TEST(SimpleTest, StringWithMove)
{
    std::string s1 = "World";
    std::string s2 = std::move(s1);
    EXPECT_EQ(s2, "World");
}

class SimpleClass
{
public:
    explicit SimpleClass(std::string name) : name_(std::move(name))
    {
    }

    std::string GetName() const
    {
        return name_;
    }

private:
    std::string name_;
};

TEST(SimpleTest, ClassWithString)
{
    SimpleClass obj("Test");
    EXPECT_EQ(obj.GetName(), "Test");
}

TEST(SimpleTest, ClassWithStringOnHeap)
{
    auto* obj = new SimpleClass("HeapTest");
    EXPECT_EQ(obj->GetName(), "HeapTest");
    delete obj;
}
