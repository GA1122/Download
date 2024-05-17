  void OnGetShareInfo(drive::FileError error,
                      scoped_ptr<drive::ResourceEntry> entry) {
    DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

    if (error != drive::FILE_ERROR_OK) {
      CompleteGetFileProperties(error);
      return;
    }

    DCHECK(entry);
    StartParseFileInfo(entry->shared_with_me());
  }
