  virtual void AddDOMDetails(
      const std::vector<SafeBrowsingHostMsg_MalwareDOMDetails_Node>& params) {
    EXPECT_TRUE(BrowserThread::CurrentlyOn(BrowserThread::IO));
    MalwareDetails::AddDOMDetails(params);

    BrowserThread::PostTask(BrowserThread::UI, FROM_HERE,
                            base::Bind(&FakeMalwareDetails::OnDOMDetailsDone,
                                       this));
  }
