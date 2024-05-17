RunOnProcessLauncherThread2() {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::PROCESS_LAUNCHER));
  BrowserThread::PostTask(
      BrowserThread::UI, FROM_HERE,
      base::Bind(&WaitForProcessLauncherThreadToGoIdleObserver::RunOnUIThread,
                 this));
}
