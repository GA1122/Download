bool IsCellularFirstDevice() {
  return base::CommandLine::ForCurrentProcess()->HasSwitch(kCellularFirst);
}
