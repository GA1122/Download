void HTMLInputElement::DispatchInputAndChangeEventIfNeeded() {
  if (isConnected() &&
      input_type_->ShouldSendChangeEventAfterCheckedChanged()) {
    DispatchInputEvent();
    DispatchChangeEvent();
  }
}
