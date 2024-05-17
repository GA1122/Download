TabletEventConverterEvdev::~TabletEventConverterEvdev() {
  Stop();
  close(fd_);
 }
