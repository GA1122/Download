void ProfileSyncComponentsFactoryImpl::RegisterDataTypes(
    ProfileSyncService* pss) {
  RegisterCommonDataTypes(pss);
#if !defined(OS_ANDROID)
  RegisterDesktopDataTypes(pss);
#endif
}
