void RenderViewImpl::OnPausePageScheduledTasks(bool paused) {
  webview()->PausePageScheduledTasks(paused);
}
