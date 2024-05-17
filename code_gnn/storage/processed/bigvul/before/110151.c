bool UserActivityDetector::PreHandleKeyEvent(aura::Window* target,
                                             aura::KeyEvent* event) {
  MaybeNotify();
  return false;
}
