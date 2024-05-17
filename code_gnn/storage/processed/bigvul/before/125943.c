FindInPageNotificationObserver::FindInPageNotificationObserver(
    AutomationProvider* automation, WebContents* parent_tab,
    bool reply_with_json, IPC::Message* reply_message)
    : automation_(automation->AsWeakPtr()),
      active_match_ordinal_(-1),
      reply_with_json_(reply_with_json),
      reply_message_(reply_message) {
  registrar_.Add(this, chrome::NOTIFICATION_FIND_RESULT_AVAILABLE,
                 content::Source<WebContents>(parent_tab));
}
