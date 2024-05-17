void VaapiVideoDecodeAccelerator::Decode(
    const media::BitstreamBuffer& bitstream_buffer) {
  DCHECK_EQ(message_loop_, base::MessageLoop::current());

  TRACE_EVENT1("Video Decoder", "VAVDA::Decode", "Buffer id",
               bitstream_buffer.id());

  MapAndQueueNewInputBuffer(bitstream_buffer);

  base::AutoLock auto_lock(lock_);
  switch (state_) {
    case kIdle:
      state_ = kDecoding;
      decoder_thread_task_runner_->PostTask(
          FROM_HERE, base::Bind(&VaapiVideoDecodeAccelerator::DecodeTask,
                                base::Unretained(this)));
      break;

    case kDecoding:
    case kResetting:
      break;

    default:
      RETURN_AND_NOTIFY_ON_FAILURE(false,
          "Decode request from client in invalid state: " << state_,
          PLATFORM_FAILURE, );
      break;
  }
}
