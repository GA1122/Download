void ComponentControllerImpl::Wait(WaitCallback callback) {
   termination_wait_callbacks_.push_back(std::move(callback));
 }
