bool InputMethodBase::DispatchKeyEventPostIME(
    const base::NativeEvent& native_event) const {
  return delegate_ ? delegate_->DispatchKeyEventPostIME(native_event) : false;
}
