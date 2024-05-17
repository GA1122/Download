void QuotaManager::GetLRUOrigin(
    StorageType type,
    const GetLRUOriginCallback& callback) {
  LazyInitialize();
  DCHECK(lru_origin_callback_.is_null());
  lru_origin_callback_ = callback;
  if (db_disabled_) {
    lru_origin_callback_.Run(GURL());
    lru_origin_callback_.Reset();
    return;
  }

  std::set<GURL>* exceptions = new std::set<GURL>;
  for (std::map<GURL, int>::const_iterator p = origins_in_use_.begin();
       p != origins_in_use_.end();
       ++p) {
    if (p->second > 0)
      exceptions->insert(p->first);
  }
  for (std::map<GURL, int>::const_iterator p = origins_in_error_.begin();
       p != origins_in_error_.end();
       ++p) {
    if (p->second > QuotaManager::kThresholdOfErrorsToBeBlacklisted)
      exceptions->insert(p->first);
  }

  GURL* url = new GURL;
  PostTaskAndReplyWithResultForDBThread(
      FROM_HERE,
      base::Bind(&GetLRUOriginOnDBThread,
                 type,
                 base::Owned(exceptions),
                 special_storage_policy_,
                 base::Unretained(url)),
      base::Bind(&QuotaManager::DidGetLRUOrigin,
                 weak_factory_.GetWeakPtr(),
                 base::Owned(url)));
}
