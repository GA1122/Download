void BrowserLauncherItemController::TabInsertedAt(
    content::WebContents* contents,
    int index,
    bool foreground) {
   UpdateAppState(contents);
 }
