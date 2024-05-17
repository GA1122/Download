void UnloadController::TabInsertedAt(content::WebContents* contents,
                                     int index,
                                     bool foreground) {
   TabAttachedImpl(contents);
 }
