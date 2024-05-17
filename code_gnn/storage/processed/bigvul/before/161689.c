void VaapiVideoDecodeAccelerator::QueueInputBuffer(
    const BitstreamBuffer& bitstream_buffer) {
  VLOGF(4) << "Queueing new input buffer id: " << bitstream_buffer.id()
           << " size: " << (int)bitstream_buffer.size();
  DCHECK(task_runner_->BelongsToCurrentThread());
  TRACE_EVENT1("Video Decoder", "QueueInputBuffer", "input_id",
               bitstream_buffer.id());

  base::AutoLock auto_lock(lock_);
  if (bitstream_buffer.size() == 0) {
    DCHECK(!base::SharedMemory::IsHandleValid(bitstream_buffer.handle()));
    auto flush_buffer = base::MakeUnique<InputBuffer>();
    DCHECK(flush_buffer->IsFlushRequest());
    input_buffers_.push(std::move(flush_buffer));
  } else {
    std::unique_ptr<SharedMemoryRegion> shm(
        new SharedMemoryRegion(bitstream_buffer, true));
    RETURN_AND_NOTIFY_ON_FAILURE(shm->Map(), "Failed to map input buffer",
                                 UNREADABLE_INPUT, );

    auto input_buffer = base::MakeUnique<InputBuffer>(
        bitstream_buffer.id(), std::move(shm),
        BindToCurrentLoop(
            base::Bind(&Client::NotifyEndOfBitstreamBuffer, client_)));
    input_buffers_.push(std::move(input_buffer));

    TRACE_COUNTER1("Video Decoder", "Input buffers", input_buffers_.size());
  }

  input_ready_.Signal();

  switch (state_) {
    case kIdle:
      state_ = kDecoding;
      decoder_thread_task_runner_->PostTask(
          FROM_HERE, base::Bind(&VaapiVideoDecodeAccelerator::DecodeTask,
                                base::Unretained(this)));
      break;

    case kDecoding:
      break;

    case kResetting:
      break;

    default:
      VLOGF(1) << "Decode/Flush request from client in invalid state: "
               << state_;
      NotifyError(PLATFORM_FAILURE);
      break;
  }
}
