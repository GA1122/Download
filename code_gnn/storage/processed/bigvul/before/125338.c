void GDataFileSystem::CheckLocalModificationAndRunAfterGetCacheEntry(
    scoped_ptr<GDataEntryProto> entry_proto,
    const GetEntryInfoCallback& callback,
    bool success,
    const GDataCacheEntry& cache_entry) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  if (!success || !cache_entry.is_dirty()) {
    if (!callback.is_null())
      callback.Run(GDATA_FILE_OK, entry_proto.Pass());
    return;
  }

  const std::string& resource_id = entry_proto->resource_id();
  const std::string& md5 = entry_proto->file_specific_info().file_md5();
  cache_->GetFileOnUIThread(
      resource_id,
      md5,
      base::Bind(
          &GDataFileSystem::CheckLocalModificationAndRunAfterGetCacheFile,
          ui_weak_ptr_, base::Passed(&entry_proto), callback));
}
