void PepperVideoRenderer2D::ApplyBuffer(const webrtc::DesktopSize& view_size,
                                        const webrtc::DesktopRect& clip_area,
                                        webrtc::DesktopFrame* buffer,
                                        const webrtc::DesktopRegion& region,
                                        const webrtc::DesktopRegion& shape) {
  DCHECK(CalledOnValidThread());

  if (!frame_received_) {
    event_handler_->OnVideoFirstFrameReceived();
    frame_received_ = true;
  }
  if (!view_size_.equals(view_size)) {
    FreeBuffer(buffer);
    AllocateBuffers();
  } else {
    FlushBuffer(clip_area, buffer, region);
    event_handler_->OnVideoShape(shape);
  }
}
