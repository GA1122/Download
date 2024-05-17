  explicit ProfilingClientBinder(content::BrowserChildProcessHost* host)
      : ProfilingClientBinder() {
    DCHECK(content::BrowserThread::CurrentlyOn(content::BrowserThread::IO));
    content::BindInterface(host->GetHost(), std::move(request_));
  }
