AppListControllerDelegate* AppListController::CreateControllerDelegate() {
  return new AppListControllerDelegateWin();
}
