 void ResourceDispatcherHost::Initialize() {
  DCHECK(ChromeThread::CurrentlyOn(ChromeThread::UI));
  webkit_thread_->Initialize();
  safe_browsing_->Initialize();
  ChromeThread::PostTask(
      ChromeThread::IO, FROM_HERE,
      NewRunnableFunction(&appcache::AppCacheInterceptor::EnsureRegistered));
}
