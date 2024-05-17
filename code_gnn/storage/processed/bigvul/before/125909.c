AllViewsStoppedLoadingObserver::AllViewsStoppedLoadingObserver(
    AutomationProvider* automation,
    IPC::Message* reply_message,
    ExtensionProcessManager* extension_process_manager)
    : automation_(automation->AsWeakPtr()),
      reply_message_(reply_message),
      extension_process_manager_(extension_process_manager) {
  registrar_.Add(this,
                 chrome::NOTIFICATION_APP_MODAL_DIALOG_SHOWN,
                 content::NotificationService::AllSources());
  registrar_.Add(this,
                 content::NOTIFICATION_LOAD_STOP,
                 content::NotificationService::AllSources());
  for (BrowserList::const_iterator iter = BrowserList::begin();
       iter != BrowserList::end();
       ++iter) {
    Browser* browser = *iter;
    for (int i = 0; i < browser->tab_count(); ++i) {
      WebContents* web_contents = chrome::GetWebContentsAt(browser, i);
      AutomationTabHelper* automation_tab_helper =
          AutomationTabHelper::FromWebContents(web_contents);
      StartObserving(automation_tab_helper);
      if (automation_tab_helper->has_pending_loads())
        pending_tabs_.insert(web_contents);
    }
  }
  CheckIfNoMorePendingLoads();
}
