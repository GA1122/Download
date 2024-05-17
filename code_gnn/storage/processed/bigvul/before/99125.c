void ResourceDispatcherHost::Shutdown() {
  DCHECK(ChromeThread::CurrentlyOn(ChromeThread::UI));
  ChromeThread::PostTask(ChromeThread::IO, FROM_HERE, new ShutdownTask(this));
}
