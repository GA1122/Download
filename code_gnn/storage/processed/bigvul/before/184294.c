  bool UserActivityDetector::PreHandleMouseEvent(aura::Window* target,
                                                 aura::MouseEvent* event) {
  MaybeNotify();
//   if (!(event->flags() & ui::EF_IS_SYNTHESIZED))
//     MaybeNotify();
    return false;
  }