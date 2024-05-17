void BrowserLauncherItemController::ActiveTabChanged(
    TabContents* old_contents,
    TabContents* new_contents,
    int index,
    bool user_gesture) {
  if (old_contents)
    UpdateAppState(old_contents->web_contents());
  UpdateAppState(new_contents->web_contents());
  UpdateLauncher(new_contents);
}
