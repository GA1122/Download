WebRtcAudioRenderer::~WebRtcAudioRenderer() {
  DCHECK_EQ(state_, UNINITIALIZED);
  buffer_.reset();
}
