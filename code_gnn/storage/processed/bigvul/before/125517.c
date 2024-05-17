void GDataDirectoryService::RefreshFile(scoped_ptr<GDataFile> fresh_file) {
  DCHECK(fresh_file.get());

  const std::string& resource_id = fresh_file->resource_id();
  GetEntryByResourceIdAsync(
      resource_id,
      base::Bind(&GDataDirectoryService::RefreshFileInternal,
                 base::Passed(&fresh_file)));
}
