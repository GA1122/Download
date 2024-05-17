void TabStrip::AddMessageLoopObserver() {
  if (!mouse_watcher_) {
    constexpr int kTabStripAnimationVSlop = 40;
    mouse_watcher_ = std::make_unique<views::MouseWatcher>(
        std::make_unique<views::MouseWatcherViewHost>(
            this, gfx::Insets(0, 0, kTabStripAnimationVSlop, 0)),
        this);
  }
  mouse_watcher_->Start(GetWidget()->GetNativeWindow());
}
