#include<iostream>
using namespace std;
#include <chrono>

class Task
{
public:
    Task();
    Task(std::string name);
    Task(void *arg);
    void createTask(void (*hander)(void* arg), void*arg);
    ~Task();
 
    void (*hander)(void* arg);
    void *arg;
    Task* next;
    std::string name;
    static int taskNum;
};
 
// task.cpp
int Task::taskNum = 0;
 
Task::Task()
{
    next = nullptr;
    this->arg = nullptr;
    taskNum++;
}
 
Task::Task(std::string name)
{
    this->name = name;
    taskNum++;
}
 
Task::Task(void *arg)
{
    next = nullptr;
    this->arg = arg;
    taskNum++;
}
 
void Task::createTask(void (*hander)(void *), void *arg)
{
    this->hander = hander;
    this->arg = arg;
}
 
Task::~Task()
{
    if(--taskNum==0) {
        // 获取当前时间点
    auto now = std::chrono::system_clock::now();

    // 转换为毫秒
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());

    // 输出毫秒数
    std::cout << "所有任务完成，完成时间: " << duration.count() << std::endl;
        
        free(this->arg);
    }
}

// threadpool.h
#include <thread>
#include <vector>
#include <condition_variable>
#include <mutex>
#include <queue>

 
typedef struct {
    Task* first;
    Task** last;
}threadPool_Queue;
 
class ThreadPool
{
 
public:
    ThreadPool(unsigned long maxNum);
    ~ThreadPool();
    void taskPost(Task* task);
 
    static void worker(void* arg);
 
private:
    //threadPool_Queue queue;
    std::queue<Task*> taskQueue;
    std::condition_variable cond;
    std::mutex mutexPool;
    unsigned long m_maxNum;
    unsigned long m_minNum;
    int busyNum;
    int aliveNum;
    std::vector<std::thread> m_threads;
};
 
// threadpool.cpp





 
ThreadPool::ThreadPool(unsigned long maxNum)
{
    m_maxNum = maxNum;
    m_minNum = 1;
 
    busyNum = 0;
    aliveNum = 0;
    //queue.first = nullptr;
    //queue.last = &queue.first;
    m_threads.resize(maxNum);
    for(unsigned long i=0; i<maxNum;i++) {
        m_threads[i] = std::thread(worker, this);
    }
}
 
ThreadPool::~ThreadPool()
{
    //唤醒阻塞的工作线程
    cond.notify_all();
    for (unsigned long i = 0; i < m_maxNum; ++i)
    {
        if (m_threads[i].joinable()) {
            m_threads[i].join();
        }
    }
}
 
void ThreadPool::taskPost(Task *task)
{
    std::unique_lock<std::mutex> lk(mutexPool);
    //task->next = nullptr;
    //*queue.last = task;
    //queue.last = &task->next;
    taskQueue.push(task);
    // 通知线程处理
    cond.notify_one();
    lk.unlock();
}
 
void ThreadPool::worker(void *arg)
{
 
    ThreadPool* pool = static_cast<ThreadPool*>(arg);
 
    while (1) {
        // unique_lock在构造时或者构造后（std::defer_lock）获取锁
        std::unique_lock<std::mutex> lk(pool->mutexPool);
        //while (!pool->queue.first) { //没有任务时，线程挂起
        while (pool->taskQueue.empty()) { //没有任务时，线程挂起
            //挂起，直到收到主线程的事件通知
            pool->cond.wait(lk);
        }
        /*Task* task = pool->queue.first;
        pool->queue.first = task->next;
        lk.unlock();
        if(pool->queue.first==nullptr )  {
            pool->queue.last=&pool->queue.first;
        }*/
        Task* task = pool->taskQueue.front();
        pool->taskQueue.pop();//从队列中移除
        // 当访问完线程池队列时，线程池解锁
        lk.unlock();
        auto now = std::chrono::system_clock::now();

    // 转换为毫秒
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());

    // 输出毫秒数
    std::cout << "当前时间 (毫秒): " << duration.count() << std::endl;
 
        task->hander(task->arg);
        std::cout << task->name << " finish!" <<std::endl;
        delete task;
    }
}
void executeTask_1(void* size) {
    int* s = static_cast<int*>(size);
    int value = *s;
    long sum=0;
    for(int i=0; i<value; i++) {
        sum+=i;
    }
    std::cout << "计算完成，sum= " <<sum << std::endl;
}
int main()
{
    long* n = new long;
    *n = 1000000000;
    // 获取当前时间
    auto start = std::chrono::system_clock::now();
    executeTask_1(n);
    executeTask_1(n);
    executeTask_1(n);
    executeTask_1(n);
    // 获取操作完成后的时间
    auto end = std::chrono::system_clock::now();
    // 计算时间差
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    // 输出时间差（以毫秒为单位）
    std::cout << "一般流程执行所有任务所需时间: " << duration.count() << "毫秒" << std::endl;
 
 
    ThreadPool pool(4);
    Task* t1 = new Task("Task1");
    t1->createTask(executeTask_1, n);
    Task* t2 = new Task("Task2");
    t2->createTask(executeTask_1, n);
    Task* t3 = new Task("Task3");
    t3->createTask(executeTask_1, n);
    Task* t4 = new Task("Task4");
    t4->createTask(executeTask_1, n);
    // 获取当前时间
    auto now = std::chrono::system_clock::now();

    // 转换为毫秒
    auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());

    // 输出毫秒数
    std::cout << "当前时间 (毫秒): " << duration1.count() << std::endl;
    pool.taskPost(t1);
    pool.taskPost(t2);
    pool.taskPost(t3);
    pool.taskPost(t4);
 
    return 0;
}