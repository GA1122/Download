WaitForProcessLauncherThreadToGoIdleObserver(
    AutomationProvider* automation, IPC::Message* reply_message)
    : automation_(automation->AsWeakPtr()),
      reply_message_(reply_message) {
  AddRef();
  BrowserThread::PostTask(
      BrowserThread::PROCESS_LAUNCHER, FROM_HERE,
      base::Bind(
          &WaitForProcessLauncherThreadToGoIdleObserver::
              RunOnProcessLauncherThread,
          this));
}
