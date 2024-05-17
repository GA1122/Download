bool IsAcceleratedVaapiVideoEncodeEnabled() {
  bool accelerated_encode_enabled = false;
#if defined(OS_CHROMEOS)
  const base::CommandLine& command_line =
      *base::CommandLine::ForCurrentProcess();
  accelerated_encode_enabled =
      !command_line.HasSwitch(switches::kDisableVaapiAcceleratedVideoEncode);
#endif
  return accelerated_encode_enabled;
}
