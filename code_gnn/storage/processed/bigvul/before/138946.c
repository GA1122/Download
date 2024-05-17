void WallpaperManagerBase::CacheUsersWallpapers() {
  DCHECK(thread_checker_.CalledOnValidThread());
  user_manager::UserList users = user_manager::UserManager::Get()->GetUsers();

  if (!users.empty()) {
    user_manager::UserList::const_iterator it = users.begin();
    it++;
    for (int cached = 0; it != users.end() && cached < kMaxWallpapersToCache;
         ++it, ++cached) {
      CacheUserWallpaper((*it)->GetAccountId());
    }
  }
}
