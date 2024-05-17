 void WtsSessionProcessDelegate::KillProcess(DWORD exit_code) {
  if (core_) {
    core_->KillProcess(exit_code);
  }
}
