#include <cstdio>
#include <ThreadPool.hpp>

void Print()
{
    std::puts("hello world");
}

void PrintInt(int value)
{
    std::printf("value is %d\n",value);
}

struct Printer
{
    void operator()() const noexcept
    {
        std::puts("function object");
    }
};

struct IntPrinter
{
    void operator()(int value) const noexcept
    {
        std::printf("function object int is %d\n",value);
    }
};


int main()
{
    ThreadPool pool{1};
    //function ptr
    pool.SubmitTask(&Print);
    pool.SubmitTask(&PrintInt,0);
    //function object
    pool.SubmitTask(Printer{});
    pool.SubmitTask(IntPrinter{},1);
    //lambda
    pool.SubmitTask([](){
        std::puts("lambda");
    });
    //compile time errors detection
    // pool.SubmitTask(1); // 1 is not a function object,function pointer or lambda
    // pool.SubmitTask([](int i){}); // i should be passed in SubmitTask
    // pool.SubmitTask([](int i){},0,1); // too many arguments were passed to SubmitTask
    return 0;
}
