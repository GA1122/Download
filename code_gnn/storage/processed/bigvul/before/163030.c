  explicit ProfilingClientBinder(content::RenderProcessHost* host)
      : ProfilingClientBinder() {
    DCHECK(content::BrowserThread::CurrentlyOn(content::BrowserThread::UI));
    content::BindInterface(host, std::move(request_));
  }
