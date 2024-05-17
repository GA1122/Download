void PepperVideoRenderer2D::ReturnBuffer(webrtc::DesktopFrame* buffer) {
  DCHECK(CalledOnValidThread());

  if (buffer->size().width() >= clip_area_.width() &&
      buffer->size().height() >= clip_area_.height()) {
    software_video_renderer_->DrawBuffer(buffer);
  } else {
    FreeBuffer(buffer);
    AllocateBuffers();
  }
}
