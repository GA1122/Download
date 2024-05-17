void VaapiVideoDecodeAccelerator::ReturnCurrInputBuffer_Locked() {
  lock_.AssertAcquired();
  DCHECK(decoder_thread_task_runner_->BelongsToCurrentThread());
  DCHECK(curr_input_buffer_.get());

  int32 id = curr_input_buffer_->id;
  curr_input_buffer_.reset();
  DVLOG(4) << "End of input buffer " << id;
  message_loop_->PostTask(FROM_HERE, base::Bind(
      &Client::NotifyEndOfBitstreamBuffer, client_, id));

  --num_stream_bufs_at_decoder_;
  TRACE_COUNTER1("Video Decoder", "Stream buffers at decoder",
                 num_stream_bufs_at_decoder_);
}
