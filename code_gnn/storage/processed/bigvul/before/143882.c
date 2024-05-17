void ComponentControllerImpl::Detach() {
  controller_binding_.set_error_handler(nullptr);
}
