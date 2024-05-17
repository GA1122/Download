  void OnUpdateViewportIntersectionOnUI(const gfx::Rect& viewport_intersection,
                                        const gfx::Rect& compositing_rect) {
    viewport_intersection_ = viewport_intersection;
    compositing_rect_ = compositing_rect;
    msg_received_ = true;
    if (run_loop_)
      run_loop_->Quit();
  }
