bool IsZipArchiverUnpackerEnabled() {
  return !base::CommandLine::ForCurrentProcess()->HasSwitch(
      kDisableZipArchiverUnpacker);
}
