void SystemKeyEventListener::GrabKey(int32 key, uint32 mask) {
  uint32 caps_lock_mask = LockMask;
  Display* display = ui::GetXDisplay();
  Window root = DefaultRootWindow(display);
  XGrabKey(display, key, mask, root, True, GrabModeAsync, GrabModeAsync);
  XGrabKey(display, key, mask | caps_lock_mask, root, True,
           GrabModeAsync, GrabModeAsync);
  XGrabKey(display, key, mask | num_lock_mask_, root, True,
           GrabModeAsync, GrabModeAsync);
  XGrabKey(display, key, mask | caps_lock_mask | num_lock_mask_, root,
           True, GrabModeAsync, GrabModeAsync);
}
