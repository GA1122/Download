 void PepperVideoRenderer2D::OnFlushDone(int result,
                                        const base::Time& paint_start,
                                        webrtc::DesktopFrame* buffer) {
  DCHECK(CalledOnValidThread());
  DCHECK(flush_pending_);

  software_video_renderer_->GetStats()->video_paint_ms()->Record(
      (base::Time::Now() - paint_start).InMilliseconds());

  flush_pending_ = false;
  ReturnBuffer(buffer);

  if (merge_buffer_) {
    buffer = merge_buffer_;
    merge_buffer_ = nullptr;
    FlushBuffer(merge_clip_area_, buffer, merge_region_);
  }
}
