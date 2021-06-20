// MIT License

// Copyright (c) 2021 YeiTingHao ShiChengFeng

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef STREAM_COMPONENT_H_
#define STREAM_COMPONENT_H_

#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

// A base class of an asynchronous FIFO task.
// Handles FIFO's operations and supports callbacks when task start, wait for
// FIFO update, process data and when task end. Example: class Encoder: public
// stream::Component<frame> {
//     void start() {
//         // init encoder
//     }
//     bool wait() {
//         // wait while queue not enpty
//         return getSize() == 0;
//     }
//     void process() {
//         // encoder frame and send data
//     }
//     void end() {
//         // clear queue
//         clearData();
//     }
// }
// Encoder enc;
// // start encode
// enc.run();
// // encode frame data
// enc.receive(frameData);
// // stop encode
// enc.stop();
template <class queueType>
class Component {
 public:
  // Constructors
  // Set thread pointer to nullptr and clear the queue
  Component();
  // Destructors
  // Clear the queue
  // Subclasses of this class should ensure stop() is called before destructor
  // is called.
  ~Component();
  // Pushes the data into queue if it was activated.
  // When received data, send a signal to continue loop.
  void receive(std::shared_ptr<queueType>);
  // Starts task and set state to activated.
  // Creates a detached thread for this task.
  void run();
  // Stops task and set state to deactivated.
  // When state is deactivated, the task will terminate automatically.
  void stop();
  // Retuns state.
  bool isActive();

 protected:
  // The main function of task.
  // Calls start function when task start.
  // Repeatly waits until wait function returns true, then call process
  // function. Calls end funuction once at the end of task.
  void loop();
  // Pushes the data into queue.
  void pushData(std::shared_ptr<queueType>);
  // Returns queue's first data and Pop the data of queue
  std::shared_ptr<queueType> getData();
  // Returns the queue's elements count.
  int getSize();
  // Removes all elements in the queue.
  void clearData();
  // A virtual function that called when task start.
  virtual void start() = 0;
  // A virtual function that returns whether it should wait for receiving data
  // or not.
  virtual bool wait() = 0;
  // A virtual function that called when wait function is over and state is
  // activated.
  virtual void process() = 0;
  // A virtual function that called at the end of task.
  virtual void end() = 0;

 private:
  // Active state.
  std::atomic_bool active;
  // The pointer of detached thread.
  std::thread *task;
  // Signal for task thread.
  std::condition_variable condition;
  // mutex lock for FIFO' IO.
  std::mutex mtx;
  // FIFO container.
  std::deque<std::shared_ptr<queueType>> data;
};

#endif  // STREAM_COMPONENT_H_
