  DriveFileStreamReaderTest()
      : ui_thread_(BrowserThread::UI),
        io_thread_(BrowserThread::IO, &message_loop_) {
  }
