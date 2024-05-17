void TestScopedKeyboardHook::LockSpecificKey(ui::DomCode dom_code) {
  keyboard_lock_active_ = true;
  locked_key_ = dom_code;
}
