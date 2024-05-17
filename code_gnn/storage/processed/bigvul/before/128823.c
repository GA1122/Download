void GLSurfaceOzoneSurfaceless::SubmitFrame() {
  DCHECK(!unsubmitted_frames_.empty());

  if (unsubmitted_frames_.front()->ready && !swap_buffers_pending_) {
    std::unique_ptr<PendingFrame> frame(unsubmitted_frames_.front());
    unsubmitted_frames_.weak_erase(unsubmitted_frames_.begin());
    swap_buffers_pending_ = true;

    if (!frame->ScheduleOverlayPlanes(widget_)) {
      frame->callback.Run(gfx::SwapResult::SWAP_FAILED);
      return;
    }

    if (ozone_surface_->IsUniversalDisplayLinkDevice())
      glFinish();

    ozone_surface_->OnSwapBuffersAsync(frame->callback);
  }
}
