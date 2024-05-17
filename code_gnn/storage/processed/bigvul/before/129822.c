static void ReleaseVideoBufferImpl(AVCodecContext* s, AVFrame* frame) {
  scoped_refptr<VideoFrame> video_frame;
  video_frame.swap(reinterpret_cast<VideoFrame**>(&frame->opaque));

  memset(frame->data, 0, sizeof(frame->data));
  frame->opaque = NULL;
}
