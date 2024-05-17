void TabStripModel::Observe(int type,
                            const content::NotificationSource& source,
                            const content::NotificationDetails& details) {
  switch (type) {
    case content::NOTIFICATION_WEB_CONTENTS_DESTROYED: {
      int index = GetIndexOfWebContents(
          content::Source<WebContents>(source).ptr());
      if (index != TabStripModel::kNoTab) {
        DetachTabContentsAt(index);
      }
      break;
    }

    case chrome::NOTIFICATION_EXTENSION_UNLOADED: {
      const extensions::Extension* extension =
          content::Details<extensions::UnloadedExtensionInfo>(
              details)->extension;
      for (int i = count() - 1; i >= 0; i--) {
        WebContents* contents = GetWebContentsAtImpl(i);
        if (extensions::TabHelper::FromWebContents(contents)->
              extension_app() == extension) {
          InternalCloseTab(contents, i, false);
        }
      }
      break;
    }

    default:
      NOTREACHED();
  }
}
