void AppListSyncableService::ProcessNewSyncItem(SyncItem* sync_item) {
  VLOG(2) << "ProcessNewSyncItem: " << sync_item->ToString();
  switch (sync_item->item_type) {
    case sync_pb::AppListSpecifics::TYPE_APP: {
      return;
    }
    case sync_pb::AppListSpecifics::TYPE_REMOVE_DEFAULT_APP: {
      VLOG(1) << this << ": Uninstall: " << sync_item->ToString();
      if (IsDriveAppSyncId(sync_item->item_id)) {
        if (drive_app_provider_) {
          drive_app_provider_->AddUninstalledDriveAppFromSync(
              GetDriveAppIdFromSyncId(sync_item->item_id));
        }
      } else {
        UninstallExtension(extension_system_->extension_service(),
                           sync_item->item_id);
      }
      return;
    }
    case sync_pb::AppListSpecifics::TYPE_FOLDER: {
      AppListItem* app_item = model_->FindItem(sync_item->item_id);
      if (!app_item)
        return;   
      UpdateAppItemFromSyncItem(sync_item, app_item);
      return;
    }
    case sync_pb::AppListSpecifics::TYPE_URL: {
      LOG(WARNING) << "TYPE_URL not supported";
      return;
    }
  }
  NOTREACHED() << "Unrecognized sync item type: " << sync_item->ToString();
}
