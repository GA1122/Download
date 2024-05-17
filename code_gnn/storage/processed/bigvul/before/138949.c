void WallpaperManagerBase::ClearDisposableWallpaperCache() {
  weak_factory_.InvalidateWeakPtrs();
  std::set<AccountId> logged_in_user_account_ids;
  const user_manager::UserList& logged_users =
      user_manager::UserManager::Get()->GetLoggedInUsers();
  for (user_manager::UserList::const_iterator it = logged_users.begin();
       it != logged_users.end(); ++it) {
    logged_in_user_account_ids.insert((*it)->GetAccountId());
  }

  CustomWallpaperMap logged_in_users_cache;
  for (CustomWallpaperMap::iterator it = wallpaper_cache_.begin();
       it != wallpaper_cache_.end(); ++it) {
    if (logged_in_user_account_ids.find(it->first) !=
        logged_in_user_account_ids.end()) {
      logged_in_users_cache.insert(*it);
    }
  }
  wallpaper_cache_ = logged_in_users_cache;
}
