bool CalledOnIOThread() {
  return BrowserThread::CurrentlyOn(BrowserThread::IO) ||
         !BrowserThread::IsMessageLoopValid(BrowserThread::IO);
}
