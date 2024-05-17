bool DaemonProcess::IsTerminalIdKnown(int terminal_id) {
  return terminal_id < next_terminal_id_;
}
