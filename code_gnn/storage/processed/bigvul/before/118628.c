  bool DoIOWork() {
    bool has_work = !fake_io_thread_work_.is_null();
    if (has_work)
      fake_io_thread_work_.Run();
    fake_io_thread_work_.Reset();
    BrowserThread::PostTask(
        BrowserThread::UI, FROM_HERE,
        base::MessageLoop::QuitWhenIdleClosure());
    return has_work;
  }
