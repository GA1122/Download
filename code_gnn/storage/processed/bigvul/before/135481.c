bool ShouldShowManualFallbackForPreLollipop(syncer::SyncService* sync_service) {
#if defined(OS_ANDROID)
  return ((base::android::BuildInfo::GetInstance()->sdk_int() >=
           base::android::SDK_VERSION_LOLLIPOP) ||
          (password_manager_util::GetPasswordSyncState(sync_service) ==
           SYNCING_NORMAL_ENCRYPTION));
#else
  return true;
#endif
}
