void BrowserRenderProcessHost::SuddenTerminationChanged(bool enabled) {
  set_sudden_termination_allowed(enabled);
}
