void ResourceMessageFilter::OnGetPlugins(bool refresh,
                                         IPC::Message* reply_msg) {
  ChromeThread::PostTask(
      ChromeThread::FILE, FROM_HERE,
      NewRunnableMethod(
          this, &ResourceMessageFilter::OnGetPluginsOnFileThread, refresh,
          reply_msg));
}
