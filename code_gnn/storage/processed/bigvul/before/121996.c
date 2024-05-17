void AppListController::ShowAppList(Profile* requested_profile) {
  DCHECK(requested_profile);
  ScopedKeepAlive show_app_list_keepalive;

  content::BrowserThread::PostBlockingPoolTask(
      FROM_HERE, base::Bind(SetDidRunForNDayActiveStats));

  if (win8::IsSingleWindowMetroMode()) {
    chrome::AppMetroInfoBarDelegateWin::Create(
        requested_profile,
        chrome::AppMetroInfoBarDelegateWin::SHOW_APP_LIST,
        std::string());
    return;
  }

  InvalidatePendingProfileLoads();

  if (IsAppListVisible() && (requested_profile == profile())) {
    current_view_->GetWidget()->Show();
    current_view_->GetWidget()->Activate();
    return;
  }

  SaveProfilePathToLocalState(requested_profile->GetPath());

  DismissAppList();
  PopulateViewFromProfile(requested_profile);

  DCHECK(current_view_);
  EnsureHaveKeepAliveForView();
  gfx::Point cursor = gfx::Screen::GetNativeScreen()->GetCursorScreenPoint();
  UpdateArrowPositionAndAnchorPoint(cursor);
  current_view_->GetWidget()->Show();
  current_view_->GetWidget()->GetTopLevelWidget()->UpdateWindowIcon();
  current_view_->GetWidget()->Activate();
  RecordAppListLaunch();
}
