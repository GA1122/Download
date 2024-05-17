void OutOfProcessInstance::OnClientTimerFired(int32_t id) {
  engine_->OnCallback(id);
}
