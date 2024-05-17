bool VaapiVideoDecodeAccelerator::GetInputBuffer_Locked() {
  DCHECK(decoder_thread_task_runner_->BelongsToCurrentThread());
  lock_.AssertAcquired();

  if (curr_input_buffer_.get())
    return true;

  while (input_buffers_.empty() && (state_ == kDecoding || state_ == kIdle)) {
    input_ready_.Wait();
  }

  if (state_ != kDecoding && state_ != kIdle)
    return false;

  DCHECK(!input_buffers_.empty());
  curr_input_buffer_ = std::move(input_buffers_.front());
  input_buffers_.pop();

  if (curr_input_buffer_->IsFlushRequest()) {
    VLOGF(4) << "New flush buffer";
    return true;
  }

  VLOGF(4) << "New current input buffer, id: " << curr_input_buffer_->id()
           << " size: " << curr_input_buffer_->shm()->size() << "B";
  decoder_->SetStream(
      static_cast<uint8_t*>(curr_input_buffer_->shm()->memory()),
      curr_input_buffer_->shm()->size());

  return true;
}
