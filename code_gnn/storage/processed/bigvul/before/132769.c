void PepperVideoRenderer2D::SetSourceSize(
    const webrtc::DesktopSize& source_size,
    const webrtc::DesktopVector& source_dpi) {
  DCHECK(CalledOnValidThread());

  if (source_size_.equals(source_size) && source_dpi_.equals(source_dpi))
    return;

  source_size_ = source_size;
  source_dpi_ = source_dpi;

  event_handler_->OnVideoSize(source_size, source_dpi);
}
