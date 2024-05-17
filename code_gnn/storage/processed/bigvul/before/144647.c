void WebContentsImpl::ReplicatePageFocus(bool is_focused) {
  if (is_being_destroyed_)
    return;

  frame_tree_.ReplicatePageFocus(is_focused);
}
