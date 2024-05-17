void Shell::PlatformCreateWindow(int width, int height) {
  CHECK(platform_);
  platform_->ResizeWindow(width, height);
}
