gfx::SwapResult GLSurfaceOzoneSurfaceless::SwapBuffers() {
  glFlush();
  if (has_implicit_external_sync_) {
    EGLSyncKHR fence = InsertFence();
    if (!fence)
      return SwapResult::SWAP_FAILED;

    EGLDisplay display = GetDisplay();
    WaitForFence(display, fence);
    eglDestroySyncKHR(display, fence);
  }

  unsubmitted_frames_.back()->ScheduleOverlayPlanes(widget_);
  unsubmitted_frames_.back()->overlays.clear();

  if (ozone_surface_->IsUniversalDisplayLinkDevice())
    glFinish();

  return ozone_surface_->OnSwapBuffers() ? gfx::SwapResult::SWAP_ACK
                                         : gfx::SwapResult::SWAP_FAILED;
}
