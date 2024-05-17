bool ClassicPendingScript::IsReady() const {
  CheckState();
  return ready_state_ >= kReady;
}
