bool IsPrintThrottlingDisabled() {
  return CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kDisableScriptedPrintThrottling);
}
