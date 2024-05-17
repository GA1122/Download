bool DisplayOldDownloadsUI() {
  return !CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kDownloadsNewUI);
}
