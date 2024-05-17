void DiscardableSharedMemoryManager::InvalidateMojoThreadWeakPtrs(
    base::WaitableEvent* event) {
  DCHECK_EQ(mojo_thread_message_loop_, base::MessageLoop::current());
  mojo_thread_weak_ptr_factory_.InvalidateWeakPtrs();
  mojo_thread_message_loop_->RemoveDestructionObserver(this);
  mojo_thread_message_loop_ = nullptr;
  if (event)
    event->Signal();
}
