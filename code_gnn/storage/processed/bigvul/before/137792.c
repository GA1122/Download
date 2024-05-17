bool AreExperimentalMuteControlsEnabled() {
  return base::CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kEnableTabAudioMuting);
}
