bool IsPrintPreviewEnabled() {
  return CommandLine::ForCurrentProcess()->HasSwitch(kEnablePrintPreview);
}
