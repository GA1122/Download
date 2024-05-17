Profile* LiveSyncTest::verifier() {
  if (verifier_.get() == NULL)
    LOG(FATAL) << "SetupClients() has not yet been called.";
  return verifier_.get();
}
