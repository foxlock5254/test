#include <iostream>
using namespace std;

class Person {
public:
    string name;
    int* age;

    // 构造函数
    Person(string n, int a) : name(n) {
        age = new int(a);
    }

    // 深拷贝构造函数
    Person(const Person& other) {
        name = other.name;               // 复制值
        age = new int(*other.age);        // 深拷贝：分配新内存并复制数据
    }

    // 析构函数
    ~Person() {
        delete age;  // 释放动态分配的内存
    }

    void display() {
        cout << "Name: " << name << ", Age: " << *age << endl;
    }
};
class Vector{
    int num;
    int* a;
public:
    // void ShallowCopy(Vector& v);
    // void DeepCopy(Vector& v);

    //左值引用形参=>匹配左值
void Copy(Vector& v){
    this->num = v.num;
    this->a = new int[num];
    for(int i=0;i<num;++i)
    {a[i]=v.a[i];}
}

//右值引用形参=>匹配右值
void Copy(Vector&& temp){
    this->num = temp.num;
    this->a = temp.a;
}

};

int main() {
    
    return 0;
}
