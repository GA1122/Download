bool AppCacheDatabase::EnsureDatabaseVersion() {
  if (!sql::MetaTable::DoesTableExist(db_.get()))
    return CreateSchema();

  if (!meta_table_->Init(db_.get(), kCurrentVersion, kCompatibleVersion))
    return false;

  if (meta_table_->GetCompatibleVersionNumber() > kCurrentVersion) {
    LOG(WARNING) << "AppCache database is too new.";
    return false;
  }

  std::string stored_flags;
  meta_table_->GetValue(kExperimentFlagsKey, &stored_flags);
  if (stored_flags != GetActiveExperimentFlags())
    return false;

  if (meta_table_->GetVersionNumber() < kCurrentVersion)
    return UpgradeSchema();

#ifndef NDEBUG
  DCHECK(sql::MetaTable::DoesTableExist(db_.get()));
  for (int i = 0; i < kTableCount; ++i) {
    DCHECK(db_->DoesTableExist(kTables[i].table_name));
  }
  for (int i = 0; i < kIndexCount; ++i) {
    DCHECK(db_->DoesIndexExist(kIndexes[i].index_name));
  }
#endif

  return true;
}
