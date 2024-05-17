    GetSyncableServiceForType(syncable::ModelType type) {
  if (!profile_) {   
     return base::WeakPtr<SyncableService>();
  }
  switch (type) {
    case syncable::PREFERENCES:
      return profile_->GetPrefs()->GetSyncableService()->AsWeakPtr();
    case syncable::AUTOFILL:
    case syncable::AUTOFILL_PROFILE: {
      if (!web_data_service_.get())
        return base::WeakPtr<SyncableService>();
      if (type == syncable::AUTOFILL) {
        return web_data_service_->GetAutocompleteSyncableService()->AsWeakPtr();
      } else {
        return web_data_service_->
                   GetAutofillProfileSyncableService()->AsWeakPtr();
      }
    }
    case syncable::APPS:
    case syncable::EXTENSIONS:
      return extension_system_->extension_service()->AsWeakPtr();
    case syncable::SEARCH_ENGINES:
      return TemplateURLServiceFactory::GetForProfile(profile_)->AsWeakPtr();
    case syncable::APP_SETTINGS:
    case syncable::EXTENSION_SETTINGS:
      return extension_system_->extension_service()->settings_frontend()->
          GetBackendForSync(type)->AsWeakPtr();
    case syncable::APP_NOTIFICATIONS:
      return extension_system_->extension_service()->
          app_notification_manager()->AsWeakPtr();
    default:
      NOTREACHED();
      return base::WeakPtr<SyncableService>();
  }
}
