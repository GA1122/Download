TriState Editor::Command::GetState(Event* triggering_event) const {
  if (!IsSupported() || !frame_)
    return kFalseTriState;
  return command_->state(*frame_, triggering_event);
}
