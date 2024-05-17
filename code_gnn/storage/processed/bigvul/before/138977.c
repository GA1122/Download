void WallpaperManagerBase::OnPolicySet(const std::string& policy,
                                       const AccountId& account_id) {
  WallpaperInfo info;
  GetUserWallpaperInfo(account_id, &info);
  info.type = POLICY;
  SetUserWallpaperInfo(account_id, info, true  );
}
