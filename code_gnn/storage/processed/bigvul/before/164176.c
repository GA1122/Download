  void FlushTasks(base::SequencedTaskRunner* runner) {
    base::WaitableEvent event(base::WaitableEvent::ResetPolicy::AUTOMATIC,
                              base::WaitableEvent::InitialState::NOT_SIGNALED);
    runner->PostTask(
        FROM_HERE,
        base::BindOnce(&AppCacheStorageImplTest::SignalEvent, &event));
    event.Wait();
  }
