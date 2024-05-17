void WaitForProcessLauncherThreadToGoIdleObserver::RunOnUIThread() {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  if (automation_)
    automation_->Send(reply_message_.release());
  Release();
}
