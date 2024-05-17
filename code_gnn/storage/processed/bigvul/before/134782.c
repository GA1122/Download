EventConverterEvdevImpl::~EventConverterEvdevImpl() {
  Stop();
  close(fd_);
 }
