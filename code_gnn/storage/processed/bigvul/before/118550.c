bool IsAppShimsEnabled() {
  return !(CommandLine::ForCurrentProcess()->HasSwitch(switches::kTestType) ||
      CommandLine::ForCurrentProcess()->HasSwitch(switches::kDisableAppShims));
}
