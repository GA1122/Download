void GDataFileSystem::OnRequestDirectoryRefresh(
    GetDocumentsParams* params,
    GDataFileError error) {
  DCHECK(params);
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  const FilePath& directory_path = params->search_file_path;
  if (error != GDATA_FILE_OK) {
    LOG(ERROR) << "Failed to refresh directory: " << directory_path.value()
               << ": " << error;
    return;
  }

  int64 unused_delta_feed_changestamp = 0;
  FeedToFileResourceMapUmaStats unused_uma_stats;
  FileResourceIdMap file_map;
  GDataWapiFeedProcessor feed_processor(directory_service_.get());
  error = feed_processor.FeedToFileResourceMap(
      *params->feed_list,
      &file_map,
      &unused_delta_feed_changestamp,
      &unused_uma_stats);
  if (error != GDATA_FILE_OK) {
    LOG(ERROR) << "Failed to convert feed: " << directory_path.value()
               << ": " << error;
    return;
  }

  directory_service_->GetEntryByResourceIdAsync(params->directory_resource_id,
      base::Bind(&GDataFileSystem::RequestDirectoryRefreshByEntry,
                 ui_weak_ptr_,
                 directory_path,
                 params->directory_resource_id,
                 file_map));
}
