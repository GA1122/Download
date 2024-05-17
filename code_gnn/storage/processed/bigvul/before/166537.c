bool BrowserCommandController::ExecuteCommand(int id) {
  return ExecuteCommandWithDisposition(id, WindowOpenDisposition::CURRENT_TAB);
}
