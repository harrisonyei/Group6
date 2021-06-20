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

#include <ctime>

#include "Components/Component.h"
#include "gtest/gtest.h"

class ComponentUT : public Component<int> {
 public:
  // Return weighted_sum
  long long getWeightedSum();

 protected:
  // Override component function
  // Reset variables
  void start();
  // Override component function
  // Return true if queue is enpty
  bool wait();
  // Override component function
  // Calculate weighted sum of input data
  void process();
  // Override component function
  // Process remain data
  void end();

 private:
  // Weighted sum of input data
  std::atomic_int64_t weighted_sum;
  // Input data counter
  std::atomic_int32_t count;
  // Flag of whether all data are processed
  std::atomic_bool finish;
};

long long ComponentUT::getWeightedSum() {
  while (!finish)
    ;
  return weighted_sum;
}

void ComponentUT::start() {
  weighted_sum = 0;
  count = 1;
  finish = false;
}

bool ComponentUT::wait() { return getSize() == 0; }

void ComponentUT::process() {
  std::shared_ptr<int> num;
  num = getData();
  weighted_sum += (*num) * (count++);
}

void ComponentUT::end() {
  while (!wait()) {
    std::shared_ptr<int> num;
    num = getData();
    weighted_sum += (*num) * (count++);
  }
  finish = true;
}

// Use weighted sum to test if Component receives data without missing and in
// the correct order.
TEST(Component, WeightedSumOfData) {
  // The number of input data generated for testing
  const int test_time = 10000;
  long long weighted_sum = 0;
  ComponentUT component_unit;
  component_unit.run();
  srand(time(NULL));
  for (int i = 1; i <= test_time; i++) {
    // Generate random input data
    int input = rand();
    // Calculate weighted sum in main thread
    weighted_sum += input * i;
    component_unit.receive(std::make_shared<int>(input));
  }
  component_unit.stop();
  // Compare weighted sum
  ASSERT_EQ(weighted_sum, component_unit.getWeightedSum());
}
