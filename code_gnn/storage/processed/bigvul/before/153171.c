void Compositor::ScheduleDraw() {
  host_->SetNeedsCommit();
}
