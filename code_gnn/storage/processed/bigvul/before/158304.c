bool RenderWidgetHostImpl::LockKeyboard() {
  if (!keyboard_lock_allowed_ || !is_focused_ || !view_)
    return false;

  base::Optional<base::flat_set<ui::DomCode>> copy = keyboard_keys_to_lock_;
  return view_->LockKeyboard(std::move(copy));
}
