jboolean ForeignSessionHelper::IsTabSyncEnabled(
    JNIEnv* env,
    const JavaParamRef<jobject>& obj) {
  sync_sessions::SessionSyncService* service =
      SessionSyncServiceFactory::GetInstance()->GetForProfile(profile_);
  return service && service->GetOpenTabsUIDelegate();
}
