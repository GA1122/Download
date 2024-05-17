  void OnGetRunningPath(drive::FileError error,
                        const base::FilePath& file_path) {
    DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

    if (error != drive::FILE_ERROR_OK) {
      StartParseFileInfo(false);
      return;
    }

    drive::FileSystemInterface* const file_system =
        drive::util::GetFileSystemByProfile(running_profile_);
    if (!file_system) {
      StartParseFileInfo(false);
      return;
    }

    file_system->GetResourceEntry(
        file_path,
        base::Bind(&SingleDriveEntryPropertiesGetter::OnGetShareInfo,
                   GetWeakPtr()));
  }
