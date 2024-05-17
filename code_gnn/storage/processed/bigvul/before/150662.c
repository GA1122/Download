void InputHandlerProxy::ScrollEndForSnapFling() {
  cc::ScrollState scroll_state = CreateScrollStateForInertialEnd();
  input_handler_->ScrollEnd(&scroll_state, false);
}
