void RenderWidgetHostViewAura::CopyFromSurfaceToVideoFrame(
    const gfx::Rect& src_subrect,
    scoped_refptr<media::VideoFrame> target,
    const base::Callback<void(const gfx::Rect&, bool)>& callback) {
  if (!IsSurfaceAvailableForCopy()) {
    callback.Run(gfx::Rect(), false);
    return;
  }
  delegated_frame_host_->CopyFromCompositingSurfaceToVideoFrame(
      src_subrect, std::move(target), callback);
}
