bool BrowserView::ShouldShowAvatar() const {
  if (!IsBrowserTypeNormal())
    return false;
  if (IsOffTheRecord() && !IsGuestSession())
    return true;
  if (ManagedMode::IsInManagedMode())
    return true;

  ProfileInfoCache& cache =
      g_browser_process->profile_manager()->GetProfileInfoCache();
  if (cache.GetIndexOfProfileWithPath(browser_->profile()->GetPath()) ==
      std::string::npos) {
    return false;
  }

  return AvatarMenuModel::ShouldShowAvatarMenu();
}
