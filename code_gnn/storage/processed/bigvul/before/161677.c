void VaapiVideoDecodeAccelerator::FlushTask() {
  VLOGF(2);
  DCHECK(decoder_thread_task_runner_->BelongsToCurrentThread());
  DCHECK(curr_input_buffer_.get() && curr_input_buffer_->IsFlushRequest());

  curr_input_buffer_.reset();

  bool res = decoder_->Flush();
  RETURN_AND_NOTIFY_ON_FAILURE(res, "Failed flushing the decoder.",
                               PLATFORM_FAILURE, );

  decoder_->Reset();

  task_runner_->PostTask(
      FROM_HERE,
      base::Bind(&VaapiVideoDecodeAccelerator::FinishFlush, weak_this_));
}
