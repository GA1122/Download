  void StartProcess() {
    DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

    file_path_ = drive::util::ExtractDrivePath(local_path_);
    file_owner_profile_ = drive::util::ExtractProfileFromPath(local_path_);

    if (!file_owner_profile_ ||
        !g_browser_process->profile_manager()->IsValidProfile(
            file_owner_profile_)) {
      CompleteGetFileProperties(drive::FILE_ERROR_FAILED);
      return;
    }

    drive::FileSystemInterface* const file_system =
        drive::util::GetFileSystemByProfile(file_owner_profile_);
    if (!file_system) {
      CompleteGetFileProperties(drive::FILE_ERROR_FAILED);
      return;
    }

    file_system->GetResourceEntry(
        file_path_,
        base::Bind(&SingleDriveEntryPropertiesGetter::OnGetFileInfo,
                   GetWeakPtr()));
  }
