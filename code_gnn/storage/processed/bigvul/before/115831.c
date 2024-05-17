  SafeBrowsingBlockingPageTest()
      : ui_thread_(BrowserThread::UI, MessageLoop::current()),
        io_thread_(BrowserThread::IO, MessageLoop::current()) {
    ResetUserResponse();
    service_ = new TestSafeBrowsingService();
  }
