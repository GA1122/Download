bool SyncTest::SetupClients() {
  if (num_clients_ <= 0)
    LOG(FATAL) << "num_clients_ incorrectly initialized.";
  if (!profiles_.empty() || !browsers_.empty() || !clients_.empty())
    LOG(FATAL) << "SetupClients() has already been called.";

  SetUpTestServerIfRequired();

  profiles_.resize(num_clients_);
  browsers_.resize(num_clients_);
  clients_.resize(num_clients_);
  for (int i = 0; i < num_clients_; ++i) {
    InitializeInstance(i);
  }

  verifier_ = MakeProfile(FILE_PATH_LITERAL("Verifier"));
  ui_test_utils::WaitForBookmarkModelToLoad(verifier()->GetBookmarkModel());
  ui_test_utils::WaitForTemplateURLServiceToLoad(
      TemplateURLServiceFactory::GetForProfile(verifier()));
  return (verifier_ != NULL);
}
