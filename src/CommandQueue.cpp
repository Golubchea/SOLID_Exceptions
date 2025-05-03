#include <CommandLib/CommandQueue.hpp>

void CommandQueue::Push(std::shared_ptr<ICommand> command) {
    std::lock_guard<std::mutex> lock(mtx_);
    queue_.push(command);
    cv_.notify_one();
}

std::shared_ptr<ICommand> CommandQueue::Pop() {
    std::unique_lock<std::mutex> lock(mtx_);
    cv_.wait(lock, [this]{ return !queue_.empty() || stopped_; });
    if (stopped_) return nullptr;
    auto cmd = queue_.front();
    queue_.pop();
    return cmd;
}

bool CommandQueue::Empty() const {
    std::lock_guard<std::mutex> lock(mtx_);
    return queue_.empty();
}

void CommandQueue::Stop() {
    std::lock_guard<std::mutex> lock(mtx_);
    stopped_ = true;
    cv_.notify_all();
}

bool CommandQueue::IsStopped() const {
    std::lock_guard<std::mutex> lock(mtx_);
    return stopped_;
}
