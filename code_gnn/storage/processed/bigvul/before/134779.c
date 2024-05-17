void EventConverterEvdevImpl::ReleaseKeys() {
  base::TimeDelta timestamp = ui::EventTimeForNow();
  for (int key = 0; key < KEY_CNT; ++key)
    OnKeyChange(key, false  , timestamp);
}
