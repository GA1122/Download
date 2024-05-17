bool InitializeTemporaryOriginsInfoOnDBThread(const std::set<GURL>* origins,
                                              QuotaDatabase* database) {
  DCHECK(database);
  if (database->IsOriginDatabaseBootstrapped())
    return true;

  if (database->RegisterInitialOriginInfo(*origins, kStorageTypeTemporary)) {
    database->SetOriginDatabaseBootstrapped(true);
    return true;
  }
  return false;
}
