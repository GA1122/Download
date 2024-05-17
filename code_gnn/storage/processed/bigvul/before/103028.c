  DeleteTabContentsOnDestroyedObserver(TabContentsWrapper* source,
                                       TabContentsWrapper* tab_to_delete)
      : source_(source),
        tab_to_delete_(tab_to_delete) {
    registrar_.Add(this,
                   NotificationType::TAB_CONTENTS_DESTROYED,
                   Source<TabContents>(source->tab_contents()));
  }
