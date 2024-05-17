  void RunTestOnIOThread(Method method) {
    test_finished_event_.reset(new base::WaitableEvent(
        base::WaitableEvent::ResetPolicy::AUTOMATIC,
        base::WaitableEvent::InitialState::NOT_SIGNALED));
    io_runner->PostTask(
        FROM_HERE,
        base::BindOnce(&AppCacheStorageImplTest::MethodWrapper<Method>,
                       base::Unretained(this), method));
    test_finished_event_->Wait();
  }
