bool Editor::Command::IsEnabled(Event* triggering_event) const {
  if (!IsSupported() || !frame_)
    return false;
  return command_->is_enabled(*frame_, triggering_event, source_);
}
