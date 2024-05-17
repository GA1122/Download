void TabHelper::Observe(int type,
                        const content::NotificationSource& source,
                        const content::NotificationDetails& details) {
  DCHECK_EQ(content::NOTIFICATION_LOAD_STOP, type);
  const NavigationController& controller =
      *content::Source<NavigationController>(source).ptr();
  DCHECK_EQ(controller.GetWebContents(), web_contents());

  if (update_shortcut_on_load_complete_) {
    update_shortcut_on_load_complete_ = false;
    if (controller.GetLastCommittedEntry())
      GetApplicationInfo(UPDATE_SHORTCUT);
  }
}
