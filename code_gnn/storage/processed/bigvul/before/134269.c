bool IsOmniboxAutoCompletionForImeEnabled() {
  return !CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kDisableOmniboxAutoCompletionForIme);
}
