void UnloadController::TabAttachedImpl(content::WebContents* contents) {
  registrar_.Add(
      this,
      content::NOTIFICATION_WEB_CONTENTS_DISCONNECTED,
       content::Source<content::WebContents>(contents));
 }
