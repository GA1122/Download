bool BrowserCommandController::UpdateCommandEnabled(int id, bool state) {
  if (is_locked_fullscreen_)
    return false;

  return command_updater_.UpdateCommandEnabled(id, state);
}
