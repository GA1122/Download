bool IsAcceleratedVideoDecodeEnabled() {
  const base::CommandLine& command_line =
      *base::CommandLine::ForCurrentProcess();
  return !command_line.HasSwitch(switches::kDisableAcceleratedVideoDecode);
}
