void QuotaManager::DidInitializeTemporaryOriginsInfo(bool success) {
  DidDatabaseWork(success);
  if (success)
     StartEviction();
 }
