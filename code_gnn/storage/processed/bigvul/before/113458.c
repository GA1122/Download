InputMethodIBus::PendingKeyEventImpl::PendingKeyEventImpl(
    InputMethodIBus* input_method,
    const base::NativeEvent& native_event,
    guint32 ibus_keyval)
    : input_method_(input_method),
      ibus_keyval_(ibus_keyval) {
  DCHECK(input_method_);

  DCHECK(native_event);
  x_event_ = *GetKeyEvent(native_event);
}
