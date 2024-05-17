bool IsVoiceInputEnabled() {
  return !base::CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kDisableVoiceInput);
}
