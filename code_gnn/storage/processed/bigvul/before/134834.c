  void FinishCopyFromCompositingSurface(const base::Closure& quit_closure,
                                        bool frame_captured) {
    ++callback_invoke_count_;
    if (frame_captured)
      ++frames_captured_;
    if (!quit_closure.is_null())
      quit_closure.Run();
  }
