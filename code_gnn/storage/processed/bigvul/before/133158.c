bool HWNDMessageHandler::RunMoveLoop(const gfx::Vector2d& drag_offset,
                                     bool hide_on_escape) {
  ReleaseCapture();
  MoveLoopMouseWatcher watcher(this, hide_on_escape);
#if defined(USE_AURA)
  base::MessageLoop::ScopedNestableTaskAllower allow_nested(
      base::MessageLoop::current());
#endif
  SendMessage(hwnd(), WM_SYSCOMMAND, SC_MOVE | 0x0002, GetMessagePos());
  return watcher.got_mouse_up();
}
