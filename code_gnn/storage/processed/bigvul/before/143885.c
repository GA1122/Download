ComponentControllerImpl::~ComponentControllerImpl() {
  for (WaitCallback& next_callback : termination_wait_callbacks_) {
    next_callback(did_terminate_abnormally_ ? 1 : 0);
  }
}
