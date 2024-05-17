  static void RequestHDRStatus() {
    BrowserThread::PostTask(BrowserThread::IO, FROM_HERE,
                            base::Bind(&HDRProxy::RequestOnIOThread));
  }
