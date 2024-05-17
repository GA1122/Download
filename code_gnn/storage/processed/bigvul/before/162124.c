  void PostTask() {
    BrowserThread::PostTask(
        BrowserThread::UI, FROM_HERE,
        base::BindOnce(&RenderProcessHostIsReadyObserver::CallTask,
                       weak_factory_.GetWeakPtr()));
  }
