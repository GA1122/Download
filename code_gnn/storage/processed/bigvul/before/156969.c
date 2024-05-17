blink::WebSize WebMediaPlayerMS::NaturalSize() const {
  DCHECK(thread_checker_.CalledOnValidThread());
  if (!video_frame_provider_)
    return blink::WebSize();

  if (video_rotation_ == media::VIDEO_ROTATION_90 ||
      video_rotation_ == media::VideoRotation::VIDEO_ROTATION_270) {
    const gfx::Size& current_size = compositor_->GetCurrentSize();
    return blink::WebSize(current_size.height(), current_size.width());
  }
  return blink::WebSize(compositor_->GetCurrentSize());
}
