void LiveSyncTest::CleanUpOnMainThread() {
  profiles_.reset();
  clients_.reset();
  verifier_.reset(NULL);
}
