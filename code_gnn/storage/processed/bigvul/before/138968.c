bool WallpaperManagerBase::IsPolicyControlled(
    const AccountId& account_id) const {
  WallpaperInfo info;
  if (!GetUserWallpaperInfo(account_id, &info))
    return false;
  return info.type == POLICY;
}
