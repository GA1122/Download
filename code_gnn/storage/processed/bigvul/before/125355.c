void GDataFileSystem::FindEntryByPathAsyncOnUIThread(
    const FilePath& search_file_path,
    const FindEntryCallback& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  if (directory_service_->origin() == INITIALIZING) {
    AddObserver(new InitialLoadObserver(
        this,
        base::Bind(&GDataFileSystem::FindEntryByPathSyncOnUIThread,
                   ui_weak_ptr_,
                   search_file_path,
                   callback)));
    return;
  } else if (directory_service_->origin() == UNINITIALIZED) {
    directory_service_->set_origin(INITIALIZING);
    feed_loader_->LoadFromCache(
        true,   
        search_file_path,
        base::Bind(&GDataFileSystem::RunAndNotifyInitialLoadFinished,
                   ui_weak_ptr_,
                   callback));
    return;
  }

  base::MessageLoopProxy::current()->PostTask(
      FROM_HERE,
      base::Bind(&GDataFileSystem::FindEntryByPathSyncOnUIThread,
                 ui_weak_ptr_,
                 search_file_path,
                 callback));
}
