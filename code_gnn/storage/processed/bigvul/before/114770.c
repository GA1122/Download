  virtual void TearDown() {
    sync_service_.reset();
    profile()->ResetRequestContext();

    DestroyBrowser();
    set_profile(NULL);

    MessageLoop::current()->RunAllPending();
    io_thread_.Stop();
    MessageLoop::current()->RunAllPending();
    BrowserWithTestWindowTest::TearDown();
  }
