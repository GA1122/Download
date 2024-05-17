void BrowserLauncherItemController::TabReplacedAt(
    TabStripModel* tab_strip_model,
    TabContents* old_contents,
    TabContents* new_contents,
    int index) {
  launcher_controller()->UpdateAppState(
      old_contents->web_contents(),
      ChromeLauncherController::APP_STATE_REMOVED);
  UpdateAppState(new_contents->web_contents());
}
