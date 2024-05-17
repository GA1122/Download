void ComponentControllerImpl::Kill() {
  did_terminate_abnormally_ = true;
  runner_->DestroyComponent(this);
}
