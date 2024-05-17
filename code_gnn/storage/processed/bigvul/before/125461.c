void RunTaskOnUIThread(const base::Closure& task) {
  RunTaskOnThread(
      BrowserThread::GetMessageLoopProxyForThread(BrowserThread::UI), task);
}
