bool GetLRUOriginOnDBThread(StorageType type,
                            std::set<GURL>* exceptions,
                            SpecialStoragePolicy* policy,
                            GURL* url,
                            QuotaDatabase* database) {
  DCHECK(database);
  database->GetLRUOrigin(type, *exceptions, policy, url);
  return true;
}
