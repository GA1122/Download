bool IsFastTabUnloadEnabled() {
  return base::CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kEnableFastUnload);
}
