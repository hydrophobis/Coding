#ifndef OCULAR_H
#define OCULAR_H

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <algorithm>
#include <ctime>
#include <random>
#include <limits>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>
#include <cstdlib>
#include <signal.h> // For SIGKILL
#include <utility>

class Threading {
public:
    // Sleep functions for different time durations
    void thread_sleep(std::chrono::milliseconds ms) {
        std::this_thread::sleep_for(ms);
    }

    void thread_sleep(std::chrono::seconds s) {
        std::this_thread::sleep_for(s);
    }

    void thread_sleep(std::chrono::microseconds mcs) {
        std::this_thread::sleep_for(mcs);
    }

    // Get thread ID
    std::thread::id thread_id() {
        return std::this_thread::get_id();
    }

    // Thread joiner
    void thread_join(std::vector<std::thread>& threads) {
        for (std::thread& t : threads) {
            if (t.joinable()) {
                t.join();
            }
        }
    }
};

struct Task {
    std::chrono::milliseconds delay;
    std::string task_path;

    void _Task(std::string task_path, std::chrono::milliseconds delay) {
        std::this_thread::sleep_for(delay);
        system(("." + task_path).c_str());
    }
};

class TaskScheduler {
public:
    TaskScheduler(int numThreads) : running(true) {
        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back(&TaskScheduler::workerThread, this);
        }
    }

    ~TaskScheduler() {
        running = false;
        cv.notify_all(); // Notify all threads to stop
        th.thread_join(threads);
    }

    void addTask(const Task& task) {
        std::lock_guard<std::mutex> lock(mutex);
        tasks.push(task);
        cv.notify_one(); // Notify one waiting thread to process the task
    }

    void startProcess(const std::string& process_path) {
        std::thread([process_path]() {
            system(process_path.c_str());
        }).detach(); // Detach thread to run independently
    }

    void stopProcess(int pid) {
        ::kill(pid, SIGKILL); // Terminate the process
    }

private:
    Threading th; // Utility object for threading operations
    std::vector<std::thread> threads;
    std::queue<Task> tasks;
    std::mutex mutex;
    std::condition_variable cv;
    std::atomic<bool> running;

    void workerThread() {
        while (running) {
            Task task;
            {
                std::unique_lock<std::mutex> lock(mutex);
                cv.wait(lock, [this]() { return !tasks.empty() || !running; });
                if (!running) return;
                task = tasks.front();
                tasks.pop();
            }
            task._Task(task.task_path, task.delay); // Corrected function call
        }
    }
};

class Log {
public:
    Log(const char* func_name) : func_name(func_name) {
        std::cout << "Entering function: " << func_name << std::endl;
    }
    ~Log() {
        std::cout << "Exiting function: " << func_name << std::endl;
    }
private:
    const char* func_name;
};

class Watch {
public:
    template<typename Func, typename... Args>
    auto track(Func func, Args&&... args) {
        auto result = func(std::forward<Args>(args)...);
        std::cout << "Function " << func() << " returned: " << result << std::endl;
        return std::make_pair(func(), result);
    }
};

// Place in a function to log anytime it is called
#define AUTOLOG Log log(__func__);

// Define a macro to track function calls
#define WATCH \
    do { \
        Watch watch; \
        auto result_pair = watch.track([](){return __func__;}); \
        auto function_name = result_pair.first; \
        auto result = result_pair.second; \
    } while (0)



#endif
