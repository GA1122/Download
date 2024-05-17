void InputHandlerProxy::WillShutdown() {
  scroll_elasticity_controller_.reset();
  input_handler_ = NULL;
  client_->WillShutdown();
}
