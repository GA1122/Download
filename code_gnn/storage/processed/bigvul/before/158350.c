bool RenderWidgetHostImpl::RequestKeyboardLock(
    base::Optional<base::flat_set<ui::DomCode>> codes) {
  if (!delegate_) {
    CancelKeyboardLock();
    return false;
  }

  DCHECK(!codes.has_value() || !codes.value().empty());
  keyboard_keys_to_lock_ = std::move(codes);
  keyboard_lock_requested_ = true;

  const bool esc_requested =
      !keyboard_keys_to_lock_.has_value() ||
      base::ContainsKey(keyboard_keys_to_lock_.value(), ui::DomCode::ESCAPE);

  if (!delegate_->RequestKeyboardLock(this, esc_requested)) {
    CancelKeyboardLock();
    return false;
  }

  return true;
}
