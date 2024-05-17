void PepperVideoRenderer2D::FreeBuffer(webrtc::DesktopFrame* buffer) {
  DCHECK(std::find(buffers_.begin(), buffers_.end(), buffer) != buffers_.end());

  buffers_.remove(buffer);
  delete buffer;
}
