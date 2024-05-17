void GDataFileSystem::CheckLocalModificationAndRun(
    scoped_ptr<GDataEntryProto> entry_proto,
    const GetEntryInfoCallback& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK(entry_proto.get());

  if (!entry_proto->has_file_specific_info() ||
      entry_proto->file_specific_info().is_hosted_document()) {
    if (!callback.is_null())
      callback.Run(GDATA_FILE_OK, entry_proto.Pass());
    return;
  }

  const std::string resource_id = entry_proto->resource_id();
  const std::string md5 = entry_proto->file_specific_info().file_md5();
  cache_->GetCacheEntryOnUIThread(
      resource_id,
      md5,
      base::Bind(
          &GDataFileSystem::CheckLocalModificationAndRunAfterGetCacheEntry,
          ui_weak_ptr_, base::Passed(&entry_proto), callback));
}
