bool Browser::ExecuteCommandIfEnabled(int id) {
  if (command_updater_.SupportsCommand(id) &&
      command_updater_.IsCommandEnabled(id)) {
    ExecuteCommand(id);
    return true;
  }
  return false;
}
