  virtual void TearDown() {
    if (SessionServiceFactory::GetForProfileIfExisting(profile()) == service())
      helper_.ReleaseService();  
    else
      helper_.set_service(NULL);
    SessionServiceFactory::SetForTestProfile(profile(), NULL);
    sync_service_.reset();
    profile()->ResetRequestContext();

    DestroyBrowser();
    set_profile(NULL);

    MessageLoop::current()->RunAllPending();
    io_thread_.Stop();
    MessageLoop::current()->RunAllPending();
    BrowserWithTestWindowTest::TearDown();
  }
