void DesktopWindowTreeHostX11::CleanUpWindowList(
    void (*func)(aura::Window* window)) {
  if (!open_windows_)
    return;
  while (!open_windows_->empty()) {
    XID xid = open_windows_->front();
    func(GetContentWindowForXID(xid));
    if (!open_windows_->empty() && open_windows_->front() == xid)
      open_windows_->erase(open_windows_->begin());
  }

  delete open_windows_;
  open_windows_ = nullptr;
}
