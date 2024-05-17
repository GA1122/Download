void GDataFileSystem::OnUpdateChecked(ContentOrigin initial_origin,
                                      GDataFileError error,
                                      GDataEntry*  ) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  if (error != GDATA_FILE_OK) {
    directory_service_->set_origin(initial_origin);
  }
}
