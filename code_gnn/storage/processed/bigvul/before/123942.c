static bool ShouldUseFixedPositionCompositing(float device_scale_factor) {
  const CommandLine& command_line = *CommandLine::ForCurrentProcess();

  if (command_line.HasSwitch(switches::kDisableCompositingForFixedPosition))
    return false;

  if (command_line.HasSwitch(switches::kEnableCompositingForFixedPosition))
    return true;

  if (device_scale_factor > 1.0f &&
      command_line.HasSwitch(
          switches::kEnableHighDpiCompositingForFixedPosition))
    return true;

  return device_scale_factor > 1.0f;
}
