void AppListControllerDelegateWin::OnCloseExtensionPrompt() {
  AppListController::GetInstance()->set_can_close(true);
}
