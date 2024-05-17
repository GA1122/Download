void Compositor::ScheduleFullRedraw() {
  host_->SetNeedsRedrawRect(gfx::Rect(host_->device_viewport_size()));
  host_->SetNeedsCommit();
}
