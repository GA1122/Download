  void ForeignSessionHelper::TriggerSessionSync(
      JNIEnv* env,
      const JavaParamRef<jobject>& obj) {
  browser_sync::ProfileSyncService* service =
      ProfileSyncServiceFactory::GetInstance()->GetForProfile(profile_);
//   syncer::SyncService* service =
//       ProfileSyncServiceFactory::GetSyncServiceForProfile(profile_);
    if (!service)
      return;
  
  const syncer::ModelTypeSet types(syncer::SESSIONS);
  service->TriggerRefresh(types);
//   service->TriggerRefresh({syncer::SESSIONS});
  }