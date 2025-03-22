#include <folly/coro/Collect.h>
#include <folly/experimental/coro/Task.h>
#include <folly/experimental/coro/Collect.h>
#include <folly/experimental/coro/BlockingWait.h>
#include <folly/futures/Future.h>
#include <folly/init/Init.h> 

#include <chrono>
#include <iostream>

using namespace std::chrono_literals;

folly::coro::Task<void> doWork(int id, std::chrono::milliseconds delay) {
    std::cout << "Task " << id << " starting\n";
    co_await folly::futures::sleep(delay);
    std::cout << "Task " << id << " finished after " << delay.count() << "ms\n";
}

folly::coro::Task<void> runAll() {
    co_await folly::coro::collectAll(
        doWork(1, 900ms),
        doWork(2, 500ms),
        doWork(3, 1100ms)
    );
}

int main(int argc, char** argv) {
    auto _ = folly::Init(&argc, &argv);
    folly::coro::blockingWait(runAll());
    return 0;
}
