#pragma once
#include <mutex>
#include <condition_variable>
#include <functional>
#include <queue>
#include <iostream>
#include <thread>

namespace base{
    class ThreadPool {
    public:
        explicit ThreadPool(size_t thread_count)
            : data_(std::make_shared<data>()) {
            for (size_t i = 0; i < thread_count; ++i) {
                std::thread([data = data_] {
                    std::unique_lock<std::mutex> lk(data->mtx_);
                    for (;;) {
                        if (!data->tasks_.empty()) {
                            auto current = std::move(data->tasks_.front());
                            data->tasks_.pop();
                            lk.unlock();
                            //std::cout<<"运行了"<<std::endl;
                            current();
                            //std::cout<<"结束了"<<std::endl;
                            lk.lock();//阻塞在任务运行结束，逻辑判断之前
                        }
                        else if (data->is_shutdown_) {
                            break;
                        }
                        else {
                            data->cond_.wait(lk);
                        }
                    }
                    }).detach();
            }
        }

        ThreadPool() = default;
        ThreadPool(ThreadPool&&) = default;

        ~ThreadPool() {
            if ((bool)data_) {
                {
                    std::lock_guard<std::mutex> lk(data_->mtx_);
                    data_->is_shutdown_ = true;
                }
                data_->cond_.notify_all();
            }
        }

        template <class F>
        void execute(F&& task) {
            {
                std::lock_guard<std::mutex> lk(data_->mtx_);
                data_->tasks_.emplace(std::forward<F>(task));
            }
            data_->cond_.notify_one();
        }

    private:
        struct data {
            std::mutex mtx_;
            std::condition_variable cond_;
            bool is_shutdown_ = false;
            std::queue<std::function<void()>> tasks_;
        };
        std::shared_ptr<data> data_;
    };
}
