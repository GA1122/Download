    GetSyncableServiceForType(syncer::ModelType type) {
  if (!profile_) {   
     return base::WeakPtr<syncer::SyncableService>();
  }
  switch (type) {
    case syncer::PREFERENCES:
      return PrefServiceSyncable::FromProfile(
          profile_)->GetSyncableService()->AsWeakPtr();
    case syncer::AUTOFILL:
    case syncer::AUTOFILL_PROFILE: {
      if (!web_data_service_.get())
        return base::WeakPtr<syncer::SyncableService>();
      if (type == syncer::AUTOFILL) {
        return AutocompleteSyncableService::FromWebDataService(
            web_data_service_)->AsWeakPtr();
      } else {
        return AutofillProfileSyncableService::FromWebDataService(
            web_data_service_)->AsWeakPtr();
      }
    }
    case syncer::APPS:
    case syncer::EXTENSIONS:
      return extension_system_->extension_service()->AsWeakPtr();
    case syncer::SEARCH_ENGINES:
      return TemplateURLServiceFactory::GetForProfile(profile_)->AsWeakPtr();
    case syncer::APP_SETTINGS:
    case syncer::EXTENSION_SETTINGS:
      return extension_system_->extension_service()->settings_frontend()->
          GetBackendForSync(type)->AsWeakPtr();
#if defined(ENABLE_THEMES)
    case syncer::THEMES:
      return ThemeServiceFactory::GetForProfile(profile_)->
          GetThemeSyncableService()->AsWeakPtr();
#endif
    case syncer::HISTORY_DELETE_DIRECTIVES: {
      HistoryService* history =
          HistoryServiceFactory::GetForProfile(
              profile_, Profile::EXPLICIT_ACCESS);
      return history ? history->AsWeakPtr() : base::WeakPtr<HistoryService>();
    }
#if !defined(OS_ANDROID)
    case syncer::SYNCED_NOTIFICATIONS: {
      notifier::ChromeNotifierService* notifier_service =
          notifier::ChromeNotifierServiceFactory::GetForProfile(
              profile_, Profile::EXPLICIT_ACCESS);
      return notifier_service ? notifier_service->AsWeakPtr()
          : base::WeakPtr<syncer::SyncableService>();
    }
#endif
    case syncer::DICTIONARY:
      return SpellcheckServiceFactory::GetForProfile(profile_)->
          GetCustomDictionary()->AsWeakPtr();
    case syncer::FAVICON_IMAGES:
    case syncer::FAVICON_TRACKING:
      return ProfileSyncServiceFactory::GetForProfile(profile_)->
          GetSessionModelAssociator()->GetFaviconCache()->AsWeakPtr();
    default:
      NOTREACHED();
      return base::WeakPtr<syncer::SyncableService>();
  }
}
