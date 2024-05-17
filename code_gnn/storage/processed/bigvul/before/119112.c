  void DidRead(File::Error error,
               const char* data,
               int bytes_read) {
    error_ = error;
    buffer_.resize(bytes_read);
    memcpy(&buffer_[0], data, bytes_read);
    MessageLoop::current()->QuitWhenIdle();
  }
