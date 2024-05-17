Editor::Command Editor::CreateCommand(const String& command_name) {
  return Command(InternalCommand(command_name), kCommandFromMenuOrKeyBinding,
                 frame_);
}
