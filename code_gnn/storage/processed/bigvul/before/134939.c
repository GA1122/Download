bool IsZipArchiverPackerEnabled() {
  return !base::CommandLine::ForCurrentProcess()->HasSwitch(
      kDisableZipArchiverPacker);
}
