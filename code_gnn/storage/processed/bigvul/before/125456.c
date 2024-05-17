void GDataFileSystem::RunAndNotifyInitialLoadFinished(
    const FindEntryCallback& callback,
    GDataFileError error,
    GDataEntry* entry) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  if (!callback.is_null())
    callback.Run(error, entry);

  DVLOG(1) << "RunAndNotifyInitialLoadFinished";

  FOR_EACH_OBSERVER(GDataFileSystemInterface::Observer, observers_,
                    OnInitialLoadFinished());
}
