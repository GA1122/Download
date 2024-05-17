bool ShouldCheckNeedDircryptoMigration() {
  return !base::CommandLine::ForCurrentProcess()->HasSwitch(
             switches::kDisableEncryptionMigration) &&
         arc::IsArcAvailable();
}
