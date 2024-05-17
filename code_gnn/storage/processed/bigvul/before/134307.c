void TabStrip::AddMessageLoopObserver() {
  if (!mouse_watcher_.get()) {
    mouse_watcher_.reset(
        new views::MouseWatcher(
            new views::MouseWatcherViewHost(
                this, gfx::Insets(0, 0, kTabStripAnimationVSlop, 0)),
            this));
  }
  mouse_watcher_->Start();
}
