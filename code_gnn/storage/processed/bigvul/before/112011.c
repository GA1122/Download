bool SyncTest::SetupSync() {
  if (profiles_.empty()) {
    if (!SetupClients())
      LOG(FATAL) << "SetupClients() failed.";
  }

  for (int i = 0; i < num_clients_; ++i) {
    if (!GetClient(i)->SetupSync())
      LOG(FATAL) << "SetupSync() failed.";
  }

  AwaitQuiescence();

  number_of_default_sync_items_ = GetClient(0)->GetNumEntries() -
                                  GetClient(0)->GetNumDatatypes() - 7;
  DVLOG(1) << "Setting " << number_of_default_sync_items_ << " as default "
           << " number of entries.";

  return true;
}
