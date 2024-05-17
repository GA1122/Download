DirOpenResult DirectoryBackingStore::Load(MetahandlesIndex* entry_bucket,
    Directory::KernelLoadInfo* kernel_load_info) {

  if (!BeginLoad())
    return FAILED_OPEN_DATABASE;

  DirOpenResult result = DoLoad(entry_bucket, kernel_load_info);

  if (result != OPENED)
    STLDeleteElements(entry_bucket);

  EndLoad();

  return result;
}
