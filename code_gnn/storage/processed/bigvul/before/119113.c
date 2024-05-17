  void DidWrite(File::Error error,
                int bytes_written) {
    error_ = error;
    bytes_written_ = bytes_written;
    MessageLoop::current()->QuitWhenIdle();
  }
