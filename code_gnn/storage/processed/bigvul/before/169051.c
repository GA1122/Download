const base::FilePath& OfflinePageModelImpl::GetArchiveDirectory(
    const std::string& name_space) const {
  if (policy_controller_->IsRemovedOnCacheReset(name_space))
    return archive_manager_->GetTemporaryArchivesDir();
   return archive_manager_->GetPersistentArchivesDir();
 }
