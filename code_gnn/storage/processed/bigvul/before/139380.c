Editor::Command Editor::CreateCommand(const String& command_name,
                                      EditorCommandSource source) {
  return Command(InternalCommand(command_name), source, frame_);
}
