void EventConverterEvdevImpl::SetAllowedKeys(
    scoped_ptr<std::set<DomCode>> allowed_keys) {
  DCHECK(HasKeyboard());
  if (!allowed_keys) {
    blocked_keys_.reset();
    return;
  }

  blocked_keys_.set();
  for (const DomCode& it : *allowed_keys) {
    int evdev_code =
        NativeCodeToEvdevCode(KeycodeConverter::DomCodeToNativeKeycode(it));
    blocked_keys_.reset(evdev_code);
  }

  base::TimeDelta timestamp = ui::EventTimeForNow();
  for (int key = 0; key < KEY_CNT; ++key) {
    if (blocked_keys_.test(key))
      OnKeyChange(key, false  , timestamp);
  }
}
