Extension::SyncType Extension::GetSyncType() const {
  if (!IsSyncable()) {
    return SYNC_TYPE_NONE;
  }

  if (!ManifestURL::GetUpdateURL(this).is_empty() && !UpdatesFromGallery())
    return SYNC_TYPE_NONE;

  if (!plugins().empty()) {
    return SYNC_TYPE_NONE;
  }

  switch (GetType()) {
    case Manifest::TYPE_EXTENSION:
      return SYNC_TYPE_EXTENSION;

    case Manifest::TYPE_USER_SCRIPT:
      if (UpdatesFromGallery())
        return SYNC_TYPE_EXTENSION;
      else
        return SYNC_TYPE_NONE;

    case Manifest::TYPE_HOSTED_APP:
    case Manifest::TYPE_LEGACY_PACKAGED_APP:
    case Manifest::TYPE_PLATFORM_APP:
        return SYNC_TYPE_APP;

    default:
      return SYNC_TYPE_NONE;
  }
}
