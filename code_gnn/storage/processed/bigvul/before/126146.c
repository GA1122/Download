void BrowserLauncherItemController::LauncherItemChanged(
    int index,
    const ash::LauncherItem& old_item) {
  if (launcher_model()->items()[index].status == ash::STATUS_ACTIVE &&
      old_item.status == ash::STATUS_RUNNING) {
    Activate();
  }
}
