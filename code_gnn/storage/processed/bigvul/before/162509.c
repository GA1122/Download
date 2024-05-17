void ClassicPendingScript::OnPurgeMemory() {
  CheckState();
  CancelStreaming();
}
