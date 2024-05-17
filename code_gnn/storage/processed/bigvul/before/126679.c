  DeleteTabContentsOnDestroyedObserver(TabContents* source,
                                       TabContents* tab_to_delete)
      : source_(source),
        tab_to_delete_(tab_to_delete) {
    registrar_.Add(this, content::NOTIFICATION_WEB_CONTENTS_DESTROYED,
                   content::Source<WebContents>(source->web_contents()));
  }
