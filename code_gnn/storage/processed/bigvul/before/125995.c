void AppLaunchObserver::Observe(int type,
                                const content::NotificationSource& source,
                                const content::NotificationDetails& details) {
  if (type == chrome::NOTIFICATION_BROWSER_WINDOW_READY) {
    new_window_id_ =
        ExtensionTabUtil::GetWindowId(content::Source<Browser>(source).ptr());
    return;
  }

  DCHECK_EQ(content::NOTIFICATION_LOAD_STOP, type);
  SessionTabHelper* session_tab_helper = SessionTabHelper::FromWebContents(
      content::Source<NavigationController>(source)->GetWebContents());
  if ((launch_container_ == extension_misc::LAUNCH_TAB) ||
      (session_tab_helper &&
          (session_tab_helper->window_id().id() == new_window_id_))) {
    if (automation_) {
      AutomationJSONReply(automation_,
                          reply_message_.release()).SendSuccess(NULL);
    }
    delete this;
  }
}
