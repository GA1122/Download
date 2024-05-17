  SiteInstanceTest()
      : ui_thread_(BrowserThread::UI, &message_loop_),
        file_user_blocking_thread_(BrowserThread::FILE_USER_BLOCKING,
                                   &message_loop_),
        io_thread_(BrowserThread::IO, &message_loop_),
        old_browser_client_(NULL) {
  }
