DiscardableSharedMemoryManager::~DiscardableSharedMemoryManager() {
  base::MemoryCoordinatorClientRegistry::GetInstance()->Unregister(this);
  base::trace_event::MemoryDumpManager::GetInstance()->UnregisterDumpProvider(
      this);

  if (mojo_thread_message_loop_) {
    if (mojo_thread_message_loop_ == base::MessageLoop::current()) {
      mojo_thread_message_loop_->RemoveDestructionObserver(this);
      mojo_thread_message_loop_ = nullptr;
    } else {
      base::WaitableEvent event(
          base::WaitableEvent::ResetPolicy::MANUAL,
          base::WaitableEvent::InitialState::NOT_SIGNALED);
      bool result = mojo_thread_message_loop_->task_runner()->PostTask(
          FROM_HERE,
          base::BindOnce(
              &DiscardableSharedMemoryManager::InvalidateMojoThreadWeakPtrs,
              base::Unretained(this), &event));
      LOG_IF(ERROR, !result) << "Invalidate mojo weak ptrs failed!";
      if (result)
        event.Wait();
    }
  }
}
