void WallpaperManagerBase::OnPolicyCleared(const std::string& policy,
                                           const AccountId& account_id) {
  WallpaperInfo info;
  GetUserWallpaperInfo(account_id, &info);
  info.type = DEFAULT;
  SetUserWallpaperInfo(account_id, info, true  );

  if (user_manager::UserManager::Get()->IsUserLoggedIn())
    SetDefaultWallpaperNow(account_id);
}
