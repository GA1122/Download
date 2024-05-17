void VaapiVideoDecodeAccelerator::FlushTask() {
  DCHECK(decoder_thread_task_runner_->BelongsToCurrentThread());
  DVLOG(1) << "Flush task";

  bool res = decoder_->Flush();
  RETURN_AND_NOTIFY_ON_FAILURE(res, "Failed flushing the decoder.",
                               PLATFORM_FAILURE, );

  decoder_->Reset();

  message_loop_->PostTask(FROM_HERE, base::Bind(
      &VaapiVideoDecodeAccelerator::FinishFlush, weak_this_));
}
