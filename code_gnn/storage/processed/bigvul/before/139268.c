void VRDisplay::StopPresenting() {
  if (is_presenting_) {
    if (!capabilities_->hasExternalDisplay()) {
      if (layer_.source().isHTMLCanvasElement()) {
      } else {
      }
    } else {
    }
    is_presenting_ = false;
    OnPresentChange();
  }

  rendering_context_ = nullptr;
  context_gl_ = nullptr;
  pending_submit_frame_ = false;
  pending_previous_frame_render_ = false;
}
