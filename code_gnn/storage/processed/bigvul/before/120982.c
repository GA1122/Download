  virtual int DoIOPending(SocketStreamEvent* event) {
    io_test_callback_.callback().Run(OK);
    return ERR_IO_PENDING;
  }
