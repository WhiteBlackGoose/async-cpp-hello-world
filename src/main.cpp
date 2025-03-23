#include <folly/coro/Collect.h>
#include <folly/coro/Mutex.h>
#include <folly/experimental/coro/Task.h>
#include <folly/experimental/coro/Collect.h>
#include <folly/experimental/coro/BlockingWait.h>
#include <folly/futures/Future.h>
#include <folly/init/Init.h> 
#include <folly/channels/Channel.h>

#include <chrono>
#include <iostream>
#include <memory>
#include <string>

using namespace std::chrono_literals;

folly::coro::Task<void> doWork(int id, std::chrono::milliseconds delay) {
    std::cout << "Task " << id << " starting\n";
    co_await folly::futures::sleep(delay);
    std::cout << "Task " << id << " finished after " << delay.count() << "ms\n";
}

folly::coro::Task<void> generator(std::shared_ptr<folly::channels::Sender<std::string>> writer, std::string message) {
    for (int i = 0; i < 10; i++) {
        co_await folly::futures::sleep(200ms);
        writer->write(message);
    }
    co_return;
}

folly::coro::Task<void> consumer(folly::channels::Receiver<std::string> reader) {
    while (auto message = co_await reader.next()) {
        if (message->empty()) {
            break;
        }
        std::cout << "Received: " << message.value() << std::endl;
    }
}

folly::coro::Task<void> runAll() {
    auto [receiver, sender] = folly::channels::Channel<std::string>::create();
    auto sender_shared = std::make_shared<decltype(sender)>(std::move(sender));
    co_await folly::coro::collectAll(
        doWork(1, 900ms),
        doWork(2, 500ms),
        doWork(3, 1100ms)
    );
    co_await folly::coro::collectAll(
        generator(sender_shared, std::string("🔥 Hello from 1st generator")),
        generator(std::move(sender_shared), std::string("🫐 Hello from 2nd generator")),
        consumer(std::move(receiver))
    );
    co_return;
}

int main(int argc, char** argv) {
    auto _ = folly::Init(&argc, &argv);
    folly::coro::blockingWait(runAll());
    return 0;
}
