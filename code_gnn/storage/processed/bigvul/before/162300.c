void CommandBufferProxyImpl::TryUpdateStateThreadSafe() {
  last_state_lock_.AssertAcquired();
  if (last_state_.error == gpu::error::kNoError) {
    shared_state()->Read(&last_state_);
    if (last_state_.error != gpu::error::kNoError) {
      callback_thread_->PostTask(
          FROM_HERE,
          base::Bind(&CommandBufferProxyImpl::LockAndDisconnectChannel,
                     weak_ptr_factory_.GetWeakPtr()));
    }
  }
}
