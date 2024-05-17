void AppListSyncableService::BuildModel() {
  CHECK(extension_system_->extension_service() &&
        extension_system_->extension_service()->is_ready());
  AppListControllerDelegate* controller = NULL;
  AppListService* service =
      AppListService::Get(chrome::HOST_DESKTOP_TYPE_NATIVE);
  if (service)
    controller = service->GetControllerDelegate();
  apps_builder_.reset(new ExtensionAppModelBuilder(controller));
  DCHECK(profile_);
  if (app_list::switches::IsAppListSyncEnabled()) {
    VLOG(1) << this << ": AppListSyncableService: InitializeWithService.";
    SyncStarted();
    apps_builder_->InitializeWithService(this, model_.get());
  } else {
    VLOG(1) << this << ": AppListSyncableService: InitializeWithProfile.";
    apps_builder_->InitializeWithProfile(profile_, model_.get());
  }

  model_pref_updater_.reset(
      new ModelPrefUpdater(AppListPrefs::Get(profile_), model_.get()));

  if (app_list::switches::IsDriveAppsInAppListEnabled())
    drive_app_provider_.reset(new DriveAppProvider(profile_, this));
}
