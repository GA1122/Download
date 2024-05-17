bool VaapiVideoDecodeAccelerator::GetInputBuffer_Locked() {
  DCHECK(decoder_thread_task_runner_->BelongsToCurrentThread());
  lock_.AssertAcquired();

  if (curr_input_buffer_.get())
    return true;

  while (input_buffers_.empty() && (state_ == kDecoding || state_ == kIdle)) {
    input_ready_.Wait();
  }

  switch (state_) {
    case kFlushing:
      if (input_buffers_.empty())
        return false;
    case kDecoding:
    case kIdle:
      DCHECK(!input_buffers_.empty());

      curr_input_buffer_ = input_buffers_.front();
      input_buffers_.pop();

      DVLOG(4) << "New current bitstream buffer, id: "
               << curr_input_buffer_->id
               << " size: " << curr_input_buffer_->size;

      decoder_->SetStream(
          static_cast<uint8*>(curr_input_buffer_->shm->memory()),
          curr_input_buffer_->size);
      return true;

    default:
      return false;
  }
}
