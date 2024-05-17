  static void GotResultOnIOThread(bool hdr_enabled) {
    BrowserThread::PostTask(BrowserThread::UI, FROM_HERE,
                            base::Bind(&HDRProxy::GotResult, hdr_enabled));
  }
