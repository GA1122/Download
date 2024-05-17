bool IsPrintPreviewEnabled() {
  return CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kRendererPrintPreview);
}
