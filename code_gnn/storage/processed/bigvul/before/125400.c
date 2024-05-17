void GDataFileSystem::OnFilePathUpdated(const FileOperationCallback& callback,
                                        GDataFileError error,
                                        const FilePath&  ) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  if (!callback.is_null())
    callback.Run(error);
}
