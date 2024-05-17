bool WakeOnWifiEnabled() {
  return !base::CommandLine::ForCurrentProcess()->HasSwitch(kDisableWakeOnWifi);
}
