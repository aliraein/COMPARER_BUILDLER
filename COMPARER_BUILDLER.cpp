// COMPARER_BUILDLE.cpp : Defines the entry point for the application.
//

#include "COMPARER_BUILDLER.h"

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <functional>

struct MyStruct
{
    int x1;
    MyStruct(int x1, int x2, std::string x3) : x1(x1), x2(x2), x3(x3){}

    int get_x2() const { return x2; }
    const std::string& get_x3() const { return x3; }
private:
    int x2;
    std::string x3;
};

template<typename T>
class comparer_builder
{
public:
    //function takes two const T&, returns a bool
    using CompareFunction = std::function<bool(const T&, const T&)>;

    template<typename Member>
    comparer_builder& by(Member member)
    {
        auto comp = [member](const T& a, const T& b)
            {
            return std::invoke(member, a) < std::invoke(member, b);
            };
        comparisons.push_back(comp);
        return *this;
    }
    bool operator()(const T& a, const T& b) const
    {
        for (const auto& comp : comparisons)
        {
            if (comp(a, b)) return true;
            if (comp(b, a)) return false;
        }
        return false; //(if all comparisons are equal)
    }
private:
    std::vector<CompareFunction> comparisons;
};

int main()
{
    std::vector<MyStruct> v{
        MyStruct(5, 10, "cplusplus"),
        MyStruct(3, 15, "freebsd"),
        MyStruct(8, 5, "hugetlbfs"),
        MyStruct(6, 14, "mimalloc"),
        MyStruct(1, 20, "hugepages"),
        MyStruct(4, 25, "visualstudio")
    };
    std::sort(v.begin(), v.end(),
        comparer_builder<MyStruct>().by(&MyStruct::x1).by(&MyStruct::get_x2));
    for (const auto& item : v)
    {
        std::cout << "x1: " << item.x1 << ", x2: " << item.get_x2() << ", x3: " << item.get_x3() << std::endl;
    }
    return 0;
}