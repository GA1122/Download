void GDataFileSystem::RequestDirectoryRefreshOnUIThreadAfterGetEntryInfo(
    const FilePath& file_path,
    GDataFileError error,
    scoped_ptr<GDataEntryProto> entry_proto) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  if (error != GDATA_FILE_OK ||
      !entry_proto->file_info().is_directory()) {
    LOG(ERROR) << "Directory entry not found: " << file_path.value();
    return;
  }

  feed_loader_->LoadFromServer(
      directory_service_->origin(),
      0,   
      0,   
      true,   
      file_path,
      std::string(),   
      GURL(),  
      entry_proto->resource_id(),
      FindEntryCallback(),   
      base::Bind(&GDataFileSystem::OnRequestDirectoryRefresh,
                 ui_weak_ptr_));
}
