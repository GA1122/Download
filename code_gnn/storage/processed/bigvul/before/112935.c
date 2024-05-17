void InsertGDataCachePathsPermissions(
    Profile* profile,
    scoped_ptr<std::vector<FilePath> > gdata_paths,
    std::vector<std::pair<FilePath, int> >* cache_paths,
    const base::Closure& callback) {
  DCHECK(profile);
  DCHECK(gdata_paths.get());
  DCHECK(cache_paths);
  DCHECK(!callback.is_null());

  GDataFileSystem* file_system = GetGDataFileSystem(profile);
  if (!file_system || gdata_paths->empty()) {
    callback.Run();
    return;
  }

  FilePath gdata_path = gdata_paths->back();
  gdata_paths->pop_back();

  file_system->GetFileInfoByPath(
      gdata_path,
      base::Bind(&OnGetFileInfoForInsertGDataCachePathsPermissions,
                 profile,
                 cache_paths,
                 base::Bind(&InsertGDataCachePathsPermissions,
                             profile,
                             base::Passed(&gdata_paths),
                             cache_paths,
                             callback)));
}
