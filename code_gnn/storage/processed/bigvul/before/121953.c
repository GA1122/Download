void AppListController::AppListClosing() {
  FreeAnyKeepAliveForView();
  current_view_ = NULL;
  view_delegate_ = NULL;
  SetProfile(NULL);
  timer_.Stop();
}
