static int GetVideoBufferImpl(AVCodecContext* s, AVFrame* frame) {
  FFmpegVideoDecoder* decoder = static_cast<FFmpegVideoDecoder*>(s->opaque);
  return decoder->GetVideoBuffer(s, frame);
}
