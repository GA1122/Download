void GpuDataManager::SetBuiltInGpuBlacklist(GpuBlacklist* built_in_list) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK(built_in_list);
  uint16 version_major, version_minor;
  bool succeed = built_in_list->GetVersion(
      &version_major, &version_minor);
  DCHECK(succeed);
  gpu_blacklist_.reset(built_in_list);
  UpdateGpuFeatureFlags();
  preliminary_gpu_feature_flags_ = gpu_feature_flags_;
  VLOG(1) << "Using software rendering list version "
          << version_major << "." << version_minor;
}
