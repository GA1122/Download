DirOpenResult DirectoryBackingStore::DoLoad(MetahandlesIndex* entry_bucket,
    Directory::KernelLoadInfo* kernel_load_info) {
  {
    DirOpenResult result = InitializeTables();
    if (result != OPENED)
      return result;
  }

  if (!DropDeletedEntries())
    return FAILED_DATABASE_CORRUPT;
  if (!LoadEntries(entry_bucket))
    return FAILED_DATABASE_CORRUPT;
  if (!LoadInfo(kernel_load_info))
    return FAILED_DATABASE_CORRUPT;

  return OPENED;
}
