Profile* SyncTest::verifier() {
  if (verifier_ == NULL)
    LOG(FATAL) << "SetupClients() has not yet been called.";
  return verifier_;
}
