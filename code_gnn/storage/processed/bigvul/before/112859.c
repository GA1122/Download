std::string CacheSubDirectoryTypeToString(
    GDataCache::CacheSubDirectoryType subdir) {
  switch (subdir) {
    case GDataCache::CACHE_TYPE_META:
      return "meta";
    case GDataCache::CACHE_TYPE_PINNED:
      return "pinned";
    case GDataCache::CACHE_TYPE_OUTGOING:
      return "outgoing";
    case GDataCache::CACHE_TYPE_PERSISTENT:
      return "persistent";
    case GDataCache::CACHE_TYPE_TMP:
      return "tmp";
    case GDataCache::CACHE_TYPE_TMP_DOWNLOADS:
      return "tmp_downloads";
    case GDataCache::CACHE_TYPE_TMP_DOCUMENTS:
      return "tmp_documents";
    case GDataCache::NUM_CACHE_TYPES:
      NOTREACHED();
  }
  NOTREACHED();
   return "unknown subdir";
 }
