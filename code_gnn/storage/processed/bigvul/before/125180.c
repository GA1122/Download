  virtual ~MockPluginProcessHostClient() {
    if (channel_)
      BrowserThread::DeleteSoon(BrowserThread::IO, FROM_HERE, channel_);
  }
