  void OnGetFileInfo(drive::FileError error,
                     scoped_ptr<drive::ResourceEntry> entry) {
    DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

    if (error != drive::FILE_ERROR_OK) {
      CompleteGetFileProperties(error);
      return;
    }

    DCHECK(entry);
    owner_resource_entry_.swap(entry);

    if (running_profile_->IsSameProfile(file_owner_profile_)) {
      StartParseFileInfo(owner_resource_entry_->shared_with_me());
      return;
    }

    drive::FileSystemInterface* const file_system =
        drive::util::GetFileSystemByProfile(running_profile_);
    if (!file_system) {
      CompleteGetFileProperties(drive::FILE_ERROR_FAILED);
      return;
    }
    file_system->GetPathFromResourceId(
        owner_resource_entry_->resource_id(),
        base::Bind(&SingleDriveEntryPropertiesGetter::OnGetRunningPath,
                   GetWeakPtr()));
  }
