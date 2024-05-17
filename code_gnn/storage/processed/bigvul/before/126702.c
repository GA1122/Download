void UnloadController::Observe(int type,
                               const content::NotificationSource& source,
                               const content::NotificationDetails& details) {
  switch (type) {
    case content::NOTIFICATION_WEB_CONTENTS_DISCONNECTED:
      if (is_attempting_to_close_browser_) {
        ClearUnloadState(content::Source<content::WebContents>(source).ptr(),
                         false);   
      }
      break;
    default:
      NOTREACHED() << "Got a notification we didn't register for.";
  }
}
