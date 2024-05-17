bool IsPrintPreviewEnabled() {
  return !CommandLine::ForCurrentProcess()->HasSwitch(kDisablePrintPreview);
}
