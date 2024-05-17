bool AboutInSettingsEnabled() {
  return SettingsWindowEnabled() &&
         !base::CommandLine::ForCurrentProcess()->HasSwitch(
             ::switches::kDisableAboutInSettings);
}
