  virtual void SendSerializedMalwareDetails(const std::string& serialized) {
    reports_.push_back(serialized);
    BrowserThread::PostTask(
        BrowserThread::UI, FROM_HERE,
        base::Bind(&FakeSafeBrowsingService::OnMalwareDetailsDone, this));
  }
