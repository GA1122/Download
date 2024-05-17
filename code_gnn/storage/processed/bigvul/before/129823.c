void FFmpegVideoDecoder::Reset(const base::Closure& closure) {
  DCHECK(task_runner_->BelongsToCurrentThread());
  DCHECK(decode_cb_.is_null());

  avcodec_flush_buffers(codec_context_.get());
  state_ = kNormal;
  task_runner_->PostTask(FROM_HERE, closure);
}
