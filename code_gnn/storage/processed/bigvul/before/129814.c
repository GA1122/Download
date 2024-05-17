bool FFmpegVideoDecoder::ConfigureDecoder(bool low_delay) {
  ReleaseFFmpegResources();

  codec_context_.reset(avcodec_alloc_context3(NULL));
  VideoDecoderConfigToAVCodecContext(config_, codec_context_.get());

  codec_context_->error_concealment = FF_EC_GUESS_MVS | FF_EC_DEBLOCK;
  codec_context_->thread_count = GetThreadCount(codec_context_->codec_id);
  codec_context_->thread_type = low_delay ? FF_THREAD_SLICE : FF_THREAD_FRAME;
  codec_context_->opaque = this;
  codec_context_->flags |= CODEC_FLAG_EMU_EDGE;
  codec_context_->get_buffer = GetVideoBufferImpl;
  codec_context_->release_buffer = ReleaseVideoBufferImpl;

  AVCodec* codec = avcodec_find_decoder(codec_context_->codec_id);
  if (!codec || avcodec_open2(codec_context_.get(), codec, NULL) < 0) {
    ReleaseFFmpegResources();
    return false;
  }

  av_frame_.reset(av_frame_alloc());
  return true;
}
