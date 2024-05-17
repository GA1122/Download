  void Init(const std::string& mime_type) {
    BrowserThread::PostTask(
        BrowserThread::IO,
        FROM_HERE,
        base::Bind(&Context::InitOnIOThread, this, mime_type));
    BrowserThread::PostDelayedTask(
        BrowserThread::IO,
        FROM_HERE,
        base::Bind(&Context::OnTimeout, this),
        base::TimeDelta::FromMilliseconds(kRemovalTimeoutMs));
  }
