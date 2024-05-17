void DiscardableSharedMemoryManager::WillDestroyCurrentMessageLoop() {
  DCHECK_EQ(mojo_thread_message_loop_, base::MessageLoop::current());
  DLOG_IF(WARNING, mojo_thread_weak_ptr_factory_.HasWeakPtrs())
      << "Some MojoDiscardableSharedMemoryManagerImpls are still alive. They "
         "will be leaked.";
  InvalidateMojoThreadWeakPtrs(nullptr);
}
