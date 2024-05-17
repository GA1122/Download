bool LiveSyncTest::SetupClients() {
  if (num_clients_ <= 0)
    LOG(FATAL) << "num_clients_ incorrectly initialized.";
  if (!profiles_.empty() || !clients_.empty())
    LOG(FATAL) << "SetupClients() has already been called.";

  SetUpTestServerIfRequired();

  for (int i = 0; i < num_clients_; ++i) {
    profiles_.push_back(MakeProfile(
        base::StringPrintf(FILE_PATH_LITERAL("Profile%d"), i)));
    EXPECT_FALSE(GetProfile(i) == NULL) << "GetProfile(" << i << ") failed.";
    clients_.push_back(
        new ProfileSyncServiceHarness(GetProfile(i), username_, password_, i));
    EXPECT_FALSE(GetClient(i) == NULL) << "GetClient(" << i << ") failed.";
  }

  verifier_.reset(MakeProfile(FILE_PATH_LITERAL("Verifier")));
  return (verifier_.get() != NULL);
}
