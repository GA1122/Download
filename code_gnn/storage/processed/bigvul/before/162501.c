void ClassicPendingScript::DisposeInternal() {
  MemoryCoordinator::Instance().UnregisterClient(this);
  ClearResource();
  integrity_failure_ = false;
  CancelStreaming();
}
