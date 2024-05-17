bool TestScopedKeyboardHook::IsKeyLocked(ui::DomCode dom_code) {
  DCHECK(keyboard_lock_active_) << "Did you forget to reserve keys to lock?";
  return !locked_key_ || (locked_key_.value() == dom_code);
}
