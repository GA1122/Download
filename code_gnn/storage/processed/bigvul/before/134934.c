bool IsSigninFrameClientCertUserSelectionEnabled() {
  return !base::CommandLine::ForCurrentProcess()->HasSwitch(
      kDisableSigninFrameClientCertUserSelection);
}
