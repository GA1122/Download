  void CallTask() {
    DCHECK_CURRENTLY_ON(BrowserThread::UI);
    if (render_process_host_->IsReady())
      std::move(task_).Run();

    delete this;
  }
