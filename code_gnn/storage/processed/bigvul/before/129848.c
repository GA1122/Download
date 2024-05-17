  void EndTraceAndFlushInThreadWithMessageLoop() {
    WaitableEvent flush_complete_event(false, false);
    Thread flush_thread("flush");
    flush_thread.Start();
    flush_thread.task_runner()->PostTask(
        FROM_HERE, base::Bind(&TraceEventTestFixture::EndTraceAndFlushAsync,
                              base::Unretained(this), &flush_complete_event));
    flush_complete_event.Wait();
  }
