bool ShouldShowShelfHoverPreviews() {
  return base::CommandLine::ForCurrentProcess()->HasSwitch(kShelfHoverPreviews);
}
