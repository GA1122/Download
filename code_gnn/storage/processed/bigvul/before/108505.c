void QuotaManager::GetOriginsModifiedSince(StorageType type,
                                           base::Time modified_since,
                                           const GetOriginsCallback& callback) {
  LazyInitialize();
  GetModifiedSinceHelper* helper = new GetModifiedSinceHelper;
  PostTaskAndReplyWithResultForDBThread(
      FROM_HERE,
      base::Bind(&GetModifiedSinceHelper::GetModifiedSinceOnDBThread,
                 base::Unretained(helper),
                 type,
                 modified_since),
      base::Bind(&GetModifiedSinceHelper::DidGetModifiedSince,
                 base::Owned(helper),
                 weak_factory_.GetWeakPtr(),
                 callback,
                 type));
}
