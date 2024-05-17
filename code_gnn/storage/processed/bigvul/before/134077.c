bool InputMethodBase::DispatchFabricatedKeyEventPostIME(EventType type,
                                                        KeyboardCode key_code,
                                                        int flags) const {
  return delegate_ ? delegate_->DispatchFabricatedKeyEventPostIME
      (type, key_code, flags) : false;
}
