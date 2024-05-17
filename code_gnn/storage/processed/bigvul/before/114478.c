EGLNativeWindowType RenderingHelper::PlatformCreateWindow(
    int top_left_x, int top_left_y) {
  HWND window = CreateWindowEx(0, L"Static", L"VideoDecodeAcceleratorTest",
                               WS_OVERLAPPEDWINDOW | WS_VISIBLE, top_left_x,
                               top_left_y, width_, height_, NULL, NULL, NULL,
                               NULL);
  CHECK(window != NULL);
  windows_.push_back(window);
  return window;
}
