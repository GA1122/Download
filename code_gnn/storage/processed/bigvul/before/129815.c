void FFmpegVideoDecoder::Decode(const scoped_refptr<DecoderBuffer>& buffer,
                                const DecodeCB& decode_cb) {
  DCHECK(task_runner_->BelongsToCurrentThread());
  DCHECK(!decode_cb.is_null());
  CHECK_NE(state_, kUninitialized);
  CHECK(decode_cb_.is_null()) << "Overlapping decodes are not supported.";
  decode_cb_ = BindToCurrentLoop(decode_cb);

  if (state_ == kError) {
    base::ResetAndReturn(&decode_cb_).Run(kDecodeError, NULL);
    return;
  }

  if (state_ == kDecodeFinished) {
    base::ResetAndReturn(&decode_cb_).Run(kOk, VideoFrame::CreateEOSFrame());
    return;
  }

  DecodeBuffer(buffer);
}
