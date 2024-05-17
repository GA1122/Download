RunOnProcessLauncherThread() {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::PROCESS_LAUNCHER));
  BrowserThread::PostTask(
      BrowserThread::PROCESS_LAUNCHER, FROM_HERE,
      base::Bind(
          &WaitForProcessLauncherThreadToGoIdleObserver::
              RunOnProcessLauncherThread2,
          this));
}
