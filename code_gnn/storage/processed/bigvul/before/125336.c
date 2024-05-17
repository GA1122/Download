void GDataFileSystem::CheckForUpdates() {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  ContentOrigin initial_origin = directory_service_->origin();
  if (initial_origin == FROM_SERVER) {
    directory_service_->set_origin(REFRESHING);
    feed_loader_->ReloadFromServerIfNeeded(
        initial_origin,
        directory_service_->largest_changestamp(),
        directory_service_->root()->GetFilePath(),
        base::Bind(&GDataFileSystem::OnUpdateChecked,
                   ui_weak_ptr_,
                   initial_origin));
  }
}
