void LoginDisplayHostWebUI::RenderProcessGone(base::TerminationStatus status) {
  if (browser_shutdown::GetShutdownType() != browser_shutdown::NOT_VALID)
    return;

  crash_count_++;
  if (crash_count_ > kCrashCountLimit)
    return;

  if (status != base::TERMINATION_STATUS_NORMAL_TERMINATION) {
    LOG(FATAL) << "Renderer crash on login window";
  }
}
