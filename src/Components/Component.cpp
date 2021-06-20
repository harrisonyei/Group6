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

#include "Component.h"

#include <assert.h>

#include <opencv2/opencv.hpp>

#include "DataType.h"

template class Component<int>;
template class Component<cv::Mat>;
template class Component<H264>;

template <class queueType>
Component<queueType>::Component() : active(false), task(nullptr) {
  clearData();
}

template <class queueType>
Component<queueType>::~Component() {
  assert(task == nullptr);
  clearData();
}

template <class queueType>
void Component<queueType>::receive(std::shared_ptr<queueType> input) {
  if (active) {
    pushData(input);
    condition.notify_all();
  }
}

template <class queueType>
void Component<queueType>::run() {
  if (!active) {
    active = true;
    task = new std::thread(&Component::loop, this);
    task->detach();
  }
}

template <class queueType>
void Component<queueType>::stop() {
  if (active) {
    active = false;
    condition.notify_all();
  }
}

template <class queueType>
bool Component<queueType>::isActive() {
  return active;
}

template <class queueType>
void Component<queueType>::loop() {
  start();
  while (active) {
    while (wait() && active) {
      std::unique_lock<std::mutex> lock(mtx);
      condition.wait(lock);
    }
    if (active) {
      process();
    }
  }
  end();
  delete task;
  task = nullptr;
}

template <class queueType>
void Component<queueType>::pushData(std::shared_ptr<queueType> input) {
  std::lock_guard<std::mutex> lock(mtx);
  data.push_back(input);
}

template <class queueType>
std::shared_ptr<queueType> Component<queueType>::getData() {
  std::lock_guard<std::mutex> lock(mtx);
  std::shared_ptr<queueType> ret;
  ret = data.front();
  data.pop_front();
  return ret;
}

template <class queueType>
int Component<queueType>::getSize() {
  std::lock_guard<std::mutex> lock(mtx);
  return data.size();
}

template <class queueType>
void Component<queueType>::clearData() {
  data.clear();
}
