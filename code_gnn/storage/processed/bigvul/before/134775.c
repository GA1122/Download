void EventConverterEvdevImpl::OnKeyChange(unsigned int key,
                                          bool down,
                                          const base::TimeDelta& timestamp) {
  if (key > KEY_MAX)
    return;

  if (down == key_state_.test(key))
    return;

  if (down && blocked_keys_.test(key))
    return;

  key_state_.set(key, down);

  dispatcher_->DispatchKeyEvent(
      KeyEventParams(input_device_.id, key, down, timestamp));
}
