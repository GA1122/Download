void TabStripModel::Observe(NotificationType type,
                            const NotificationSource& source,
                            const NotificationDetails& details) {
  switch (type.value) {
    case NotificationType::TAB_CONTENTS_DESTROYED: {
      int index = GetWrapperIndex(Source<TabContents>(source).ptr());
      if (index != TabStripModel::kNoTab) {
        DetachTabContentsAt(index);
      }
      break;
    }

    case NotificationType::EXTENSION_UNLOADED: {
      const Extension* extension =
          Details<UnloadedExtensionInfo>(details)->extension;
      for (int i = count() - 1; i >= 0; i--) {
        TabContentsWrapper* contents = GetTabContentsAt(i);
        if (contents->extension_tab_helper()->extension_app() == extension) {

          InternalCloseTab(contents, i, false);
        }
      }
      break;
    }

    default:
      NOTREACHED();
  }
}
