bool Editor::Command::Execute(Event* triggering_event) const {
  return Execute(String(), triggering_event);
}
