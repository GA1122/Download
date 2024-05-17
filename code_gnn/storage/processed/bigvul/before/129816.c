void FFmpegVideoDecoder::DecodeBuffer(
    const scoped_refptr<DecoderBuffer>& buffer) {
  DCHECK(task_runner_->BelongsToCurrentThread());
  DCHECK_NE(state_, kUninitialized);
  DCHECK_NE(state_, kDecodeFinished);
  DCHECK_NE(state_, kError);
  DCHECK(!decode_cb_.is_null());
  DCHECK(buffer);


  if (state_ == kNormal && buffer->end_of_stream()) {
    state_ = kFlushCodec;
  }

  scoped_refptr<VideoFrame> video_frame;
  if (!FFmpegDecode(buffer, &video_frame)) {
    state_ = kError;
    base::ResetAndReturn(&decode_cb_).Run(kDecodeError, NULL);
    return;
  }

  if (!video_frame.get()) {
    if (state_ == kFlushCodec) {
      DCHECK(buffer->end_of_stream());
      state_ = kDecodeFinished;
      base::ResetAndReturn(&decode_cb_)
          .Run(kOk, VideoFrame::CreateEOSFrame());
      return;
    }

    base::ResetAndReturn(&decode_cb_).Run(kNotEnoughData, NULL);
    return;
  }

  base::ResetAndReturn(&decode_cb_).Run(kOk, video_frame);
}
