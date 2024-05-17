 std::string CacheSubDirectoryTypeToString(
     GDataRootDirectory::CacheSubDirectoryType subdir) {
  switch (subdir) {
    case GDataRootDirectory::CACHE_TYPE_META: return "meta";
    case GDataRootDirectory::CACHE_TYPE_PINNED: return "pinned";
    case GDataRootDirectory::CACHE_TYPE_OUTGOING: return "outgoing";
    case GDataRootDirectory::CACHE_TYPE_PERSISTENT: return "persistent";
    case GDataRootDirectory::CACHE_TYPE_TMP: return "tmp";
    case GDataRootDirectory::CACHE_TYPE_TMP_DOWNLOADS:
      return "tmp_downloads";
    case GDataRootDirectory::CACHE_TYPE_TMP_DOCUMENTS:
      return "tmp_documents";
    case GDataRootDirectory::NUM_CACHE_TYPES:
      NOTREACHED();
  }
  NOTREACHED();
  return "unknown subdir";
}
