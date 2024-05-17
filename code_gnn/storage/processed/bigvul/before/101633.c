void Browser::ExecuteCommand(int id, int event_flags) {
  ExecuteCommandWithDisposition(
      id, browser::DispositionFromEventFlags(event_flags));
}
