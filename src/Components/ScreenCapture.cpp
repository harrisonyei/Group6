// MIT License

// Copyright (c) 2021 YeiTingHao ShiChengFeng

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "ScreenCapture.h"

#include "Decoder.h"
#include "Encoder.h"
#include "Render.h"

ScreenCapture::ScreenCapture(Render* render_, Encoder* encoder_) : Component() {
    render = render_;
    encoder = encoder_;
    // initialize
    hwnd = NULL;
    hwindowDC = NULL;
    hwindowCompatibleDC = NULL;
    bitmap = NULL;
    // assign image width
    img_width = 1080;
}

void ScreenCapture::start() {
    setupWindow();
}

bool ScreenCapture::wait() {
    return false;
}

void ScreenCapture::process() {
    std::shared_ptr<cv::Mat> framePtr(new cv::Mat());
    // capture image
    (*framePtr) = captureScreenMat(hwnd);
    // send to renderer and encoder
    if (!framePtr->empty()) {
        render->receive(framePtr);
        encoder->receive(framePtr);
    }
}

void ScreenCapture::end() {
    releaseWindow();
}

void ScreenCapture::setupWindow(){
    hwnd = GetDesktopWindow();
    if(!hwnd){
        return;
    }
    // get handles to a device context (DC)
    hwindowDC = GetDC(hwnd);
    if(!hwindowDC){
        return;
    }
    hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
    if(!hwindowCompatibleDC){
        return;
    }
    SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);
    // get window configuration
    window_config.x = GetSystemMetrics(SM_XVIRTUALSCREEN);
    window_config.y = GetSystemMetrics(SM_YVIRTUALSCREEN);
    window_config.w = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    window_config.h = GetSystemMetrics(SM_CYVIRTUALSCREEN);

    HMONITOR monitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
    MONITORINFO info;
    info.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(monitor, &info);

    monitor_config.x = info.rcMonitor.left;
    monitor_config.y = info.rcMonitor.top;
    monitor_config.w = info.rcMonitor.right - info.rcMonitor.left;
    monitor_config.h = info.rcMonitor.bottom - info.rcMonitor.top;

    // calculate image height accoring to given width
    // img_width = 1080;
    img_height = img_width * (monitor_config.h / (float)monitor_config.w);

    // create a bitmap
    bitmap = CreateCompatibleBitmap(hwindowDC, img_width, img_height);
    if(!bitmap){
        return;
    }
    // create a bitmap header
    bitmap_info.biSize = sizeof(BITMAPINFOHEADER);
    bitmap_info.biWidth = img_width;
    bitmap_info.biHeight = -img_height;  //this is the line that makes it draw upside down or not
    bitmap_info.biPlanes = 1;
    bitmap_info.biBitCount = 32;
    bitmap_info.biCompression = BI_RGB;
    bitmap_info.biSizeImage = 0;
    bitmap_info.biXPelsPerMeter = 0;
    bitmap_info.biYPelsPerMeter = 0;
    bitmap_info.biClrUsed = 0;
    bitmap_info.biClrImportant = 0;
}

void ScreenCapture::releaseWindow(){
    if(bitmap){
        DeleteObject(bitmap);
    }
    if(hwindowCompatibleDC){
        DeleteDC(hwindowCompatibleDC);
    }
    if(hwnd){
        ReleaseDC(hwnd, hwindowDC);
    }
}

cv::Mat ScreenCapture::captureScreenMat()
{
    if(!hwnd || !hwindowDC || !hwindowCompatibleDC || !bitmap){
        return;
    }
	// create mat object
    cv::Mat src;
    src.create(img_height, img_width, CV_8UC4);
    // use the previously created device context with the bitmap
    SelectObject(hwindowCompatibleDC, bitmap);
    // copy from the window device context to the bitmap device context
    StretchBlt(hwindowCompatibleDC, 0, 0, img_width, img_height,
        hwindowDC, monitor_config.x, monitor_config.y, monitor_config.w, monitor_config.h, SRCCOPY);
    //copy from hwindowCompatibleDC to bitmap
    GetDIBits(hwindowCompatibleDC, bitmap, 0, img_height, src.data,
        (BITMAPINFO*)&bitmap_info, DIB_RGB_COLORS);
    return src;
}
