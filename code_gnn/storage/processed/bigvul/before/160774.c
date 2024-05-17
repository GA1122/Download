void RenderViewImpl::DidCommitProvisionalHistoryLoad() {
  history_navigation_virtual_time_pauser_.PauseVirtualTime(false);
}
