void TestScopedKeyboardHook::LockAllKeys() {
  keyboard_lock_active_ = true;
  locked_key_.reset();
}
