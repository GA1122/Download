void Automation::Terminate() {
  if (launcher_.get() && launcher_->process() != base::kNullProcessHandle) {
    launcher_->QuitBrowser();
  }
}
