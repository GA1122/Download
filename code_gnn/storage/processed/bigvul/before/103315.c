void GpuDataManager::UpdateGpuBlacklist(
    GpuBlacklist* gpu_blacklist, bool preliminary) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK(gpu_blacklist);

  scoped_ptr<GpuBlacklist> updated_list(gpu_blacklist);

  uint16 updated_version_major, updated_version_minor;
  if (!updated_list->GetVersion(
          &updated_version_major, &updated_version_minor))
    return;

  uint16 current_version_major, current_version_minor;
  bool succeed = gpu_blacklist_->GetVersion(
      &current_version_major, &current_version_minor);
  DCHECK(succeed);
  if (updated_version_major < current_version_major ||
      (updated_version_major == current_version_major &&
       updated_version_minor <= current_version_minor))
    return;

  gpu_blacklist_.reset(updated_list.release());
  UpdateGpuFeatureFlags();
  if (preliminary)
    preliminary_gpu_feature_flags_ = gpu_feature_flags_;
  VLOG(1) << "Using software rendering list version "
          << updated_version_major << "." << updated_version_minor;
}
