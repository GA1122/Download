bool IsSigninFrameClientCertsEnabled() {
  return !base::CommandLine::ForCurrentProcess()->HasSwitch(
      kDisableSigninFrameClientCerts);
}
