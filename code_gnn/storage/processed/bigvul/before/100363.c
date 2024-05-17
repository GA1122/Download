void BrowserRenderProcessHost::BadMessageTerminateProcess(
    uint16 msg_type, base::ProcessHandle process) {
  LOG(ERROR) << "bad message " << msg_type << " terminating renderer.";
  if (BrowserRenderProcessHost::run_renderer_in_process()) {
    CHECK(false);
  }
  NOTREACHED();
  base::KillProcess(process, ResultCodes::KILLED_BAD_MESSAGE, false);
}
