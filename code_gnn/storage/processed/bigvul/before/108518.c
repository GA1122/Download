void QuotaManager::LazyInitialize() {
  DCHECK(io_thread_->BelongsToCurrentThread());
  if (database_.get()) {
    return;
  }

  database_.reset(new QuotaDatabase(is_incognito_ ? FilePath() :
      profile_path_.AppendASCII(kDatabaseName)));

  temporary_usage_tracker_.reset(
      new UsageTracker(clients_, kStorageTypeTemporary,
                       special_storage_policy_));
  persistent_usage_tracker_.reset(
      new UsageTracker(clients_, kStorageTypePersistent,
                       special_storage_policy_));

  int64* temporary_quota_override = new int64(-1);
  int64* desired_available_space = new int64(-1);
  PostTaskAndReplyWithResultForDBThread(
      FROM_HERE,
      base::Bind(&InitializeOnDBThread,
                 base::Unretained(temporary_quota_override),
                 base::Unretained(desired_available_space)),
      base::Bind(&QuotaManager::DidInitialize,
                 weak_factory_.GetWeakPtr(),
                 base::Owned(temporary_quota_override),
                 base::Owned(desired_available_space)));
}
