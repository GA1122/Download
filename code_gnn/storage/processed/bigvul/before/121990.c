void AppListControllerDelegateWin::OnShowExtensionPrompt() {
  AppListController::GetInstance()->set_can_close(false);
}
