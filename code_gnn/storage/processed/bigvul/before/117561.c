void SystemKeyEventListener::OnCapsLock(bool enabled) {
  FOR_EACH_OBSERVER(
      CapsLockObserver, caps_lock_observers_, OnCapsLockChange(enabled));
}
