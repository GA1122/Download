  void OnUpdateViewportIntersectionOnUI(const gfx::Rect& viewport_intersection,
                                        const gfx::Rect& compositing_rect,
                                        bool occluded_or_obscured) {
    viewport_intersection_ = viewport_intersection;
    compositing_rect_ = compositing_rect;
    occluded_or_obscured_ = occluded_or_obscured;
    msg_received_ = true;
    if (run_loop_)
      run_loop_->Quit();
  }
