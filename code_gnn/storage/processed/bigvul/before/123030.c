void RenderWidgetHostImpl::ProcessKeyboardEventAck(int type, bool processed) {
  if (key_queue_.empty()) {
    LOG(ERROR) << "Got a KeyEvent back from the renderer but we "
               << "don't seem to have sent it to the renderer!";
  } else if (key_queue_.front().type != type) {
    LOG(ERROR) << "We seem to have a different key type sent from "
               << "the renderer. (" << key_queue_.front().type << " vs. "
               << type << "). Ignoring event.";

    key_queue_.clear();
    suppress_next_char_events_ = false;
  } else {
    NativeWebKeyboardEvent front_item = key_queue_.front();
    key_queue_.pop_front();

#if defined(OS_MACOSX)
    if (!is_hidden_ && view_->PostProcessEventForPluginIme(front_item))
      return;
#endif

    if (delegate_ && !processed && !is_hidden_ && !front_item.skip_in_browser) {
      delegate_->HandleKeyboardEvent(front_item);

    }
  }
}
