void ModifyGDataFileResourceUrl(Profile* profile,
                                const FilePath& gdata_cache_path,
                                GURL* url) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  GDataFileSystem* file_system = GetGDataFileSystem(profile);
  if (!file_system)
    return;
  GDataCache* cache = GetGDataCache(profile);
  if (!cache)
    return;

  if (cache->GetCacheDirectoryPath(
          GDataCache::CACHE_TYPE_TMP_DOCUMENTS).IsParent(
      gdata_cache_path)) {
    GURL* edit_url = new GURL();
    content::BrowserThread::GetBlockingPool()->PostTaskAndReply(FROM_HERE,
        base::Bind(&GetHostedDocumentURLBlockingThread,
                   gdata_cache_path, edit_url),
        base::Bind(&OpenEditURLUIThread, profile, base::Owned(edit_url)));
    *url = GURL();
  } else if (cache->GetCacheDirectoryPath(
      GDataCache::CACHE_TYPE_TMP).IsParent(gdata_cache_path)) {
    const std::string resource_id =
        gdata_cache_path.BaseName().RemoveExtension().AsUTF8Unsafe();
    file_system->GetFileInfoByResourceId(
        resource_id,
        base::Bind(&OnGetFileInfoByResourceId,
                   profile,
                   resource_id));
    *url = GURL();
  }
}
