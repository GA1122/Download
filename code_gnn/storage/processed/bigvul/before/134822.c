TouchEventConverterEvdev::~TouchEventConverterEvdev() {
  Stop();
  close(fd_);
}
