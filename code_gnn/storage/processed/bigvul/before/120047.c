  void StartParseFileInfo(bool shared_with_me) {
    DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

    FillDriveEntryPropertiesValue(
        *owner_resource_entry_, shared_with_me, properties_.get());

    drive::FileSystemInterface* const file_system =
        drive::util::GetFileSystemByProfile(file_owner_profile_);
    drive::DriveAppRegistry* const app_registry =
        drive::util::GetDriveAppRegistryByProfile(file_owner_profile_);
    if (!file_system || !app_registry) {
      CompleteGetFileProperties(drive::FILE_ERROR_FAILED);
      return;
    }

    if (!owner_resource_entry_->has_file_specific_info()) {
      CompleteGetFileProperties(drive::FILE_ERROR_OK);
      return;
    }

    const drive::FileSpecificInfo& file_specific_info =
        owner_resource_entry_->file_specific_info();

    std::vector<drive::DriveAppInfo> drive_apps;
    app_registry->GetAppsForFile(file_path_.Extension(),
                                 file_specific_info.content_mime_type(),
                                 &drive_apps);
    if (!drive_apps.empty()) {
      std::string default_task_id =
          file_manager::file_tasks::GetDefaultTaskIdFromPrefs(
              *file_owner_profile_->GetPrefs(),
              file_specific_info.content_mime_type(),
              file_path_.Extension());
      file_manager::file_tasks::TaskDescriptor default_task;
      file_manager::file_tasks::ParseTaskID(default_task_id, &default_task);
      DCHECK(default_task_id.empty() || !default_task.app_id.empty());
      for (size_t i = 0; i < drive_apps.size(); ++i) {
        const drive::DriveAppInfo& app_info = drive_apps[i];
        if (default_task.app_id == app_info.app_id) {
          const GURL doc_icon = drive::util::FindPreferredIcon(
              app_info.document_icons, drive::util::kPreferredIconSize);
          properties_->custom_icon_url.reset(new std::string(doc_icon.spec()));
        }
      }
    }

    CompleteGetFileProperties(drive::FILE_ERROR_OK);
  }
