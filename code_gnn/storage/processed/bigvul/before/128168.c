  Context(base::Time begin_time, BrowserContext* browser_context)
      : event_(new base::WaitableEvent(true, false)),
        begin_time_(begin_time),
        is_removing_(false),
        browser_context_path_(browser_context->GetPath()),
        resource_context_(browser_context->GetResourceContext()) {
    DCHECK_CURRENTLY_ON(BrowserThread::UI);
  }
