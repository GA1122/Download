  std::string GetRawNondefaultId() {
    std::string id;
    BrowserThread::PostTask(
        BrowserThread::IO, FROM_HERE,
        base::Bind(
            &AudioOutputAuthorizationHandlerTest::GetRawNondefaultIdOnIOThread,
            base::Unretained(this), base::Unretained(&id)));
    SyncWithAllThreads();
    return id;
  }
