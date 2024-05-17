void QuitUIMessageLoopFromIOThread() {
  BrowserThread::PostTask(BrowserThread::UI, FROM_HERE,
                          MessageLoop::QuitClosure());
}
