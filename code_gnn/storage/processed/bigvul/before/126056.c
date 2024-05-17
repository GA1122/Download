void TabCountChangeObserver::TabInsertedAt(WebContents* contents,
                                           int index,
                                           bool foreground) {
   CheckTabCount();
 }
