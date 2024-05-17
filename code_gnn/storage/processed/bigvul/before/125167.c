  void QuitMessageLoop() {
    BrowserThread::PostTask(BrowserThread::UI, FROM_HERE,
                            MessageLoop::QuitClosure());
  }
