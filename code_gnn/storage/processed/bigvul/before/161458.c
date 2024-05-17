void TargetHandler::Throttle::Clear() {
  CleanupPointers();
  if (agent_host_) {
    agent_host_ = nullptr;
    Resume();
  }
}
