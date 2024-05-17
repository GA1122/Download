void Layer::SetShouldScrollOnMainThread(bool should_scroll_on_main_thread) {
  DCHECK(IsPropertyChangeAllowed());
  if (should_scroll_on_main_thread_ == should_scroll_on_main_thread)
    return;
  should_scroll_on_main_thread_ = should_scroll_on_main_thread;
  SetNeedsCommit();
}
