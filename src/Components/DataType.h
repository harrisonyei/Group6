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

#ifndef STREAM_DATATYPE_H_
#define STREAM_DATATYPE_H_

// Libav encode using config
struct EncodeConfig {
  // video width
  int width = 1280 * 0.5;
  // video height
  int height = 720 * 0.5;
  // video encode frames per second
  int frame_rate = 30;
  // I frame interval
  int gop_size = 100;
  // B frame rate
  int max_b_frames = 0;
};

// After encode packet
struct H264 {
  // Size of data bytes
  int size;
  // Packet data pointer
  uint8_t *data;
  // Destructors, auto free data
  ~H264() {
    if (data) {
      delete[] data;
    }
    data = nullptr;
  }
};

#endif  // STREAM_DATATYPE_H_
