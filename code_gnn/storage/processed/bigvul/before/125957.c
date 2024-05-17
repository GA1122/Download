InfoBarCountObserver::InfoBarCountObserver(AutomationProvider* automation,
                                           IPC::Message* reply_message,
                                           TabContents* tab_contents,
                                           size_t target_count)
    : automation_(automation->AsWeakPtr()),
      reply_message_(reply_message),
      tab_contents_(tab_contents),
      target_count_(target_count) {
  content::Source<InfoBarTabHelper> source(
      InfoBarTabHelper::FromWebContents(tab_contents->web_contents()));
  registrar_.Add(this, chrome::NOTIFICATION_TAB_CONTENTS_INFOBAR_ADDED,
                 source);
  registrar_.Add(this, chrome::NOTIFICATION_TAB_CONTENTS_INFOBAR_REMOVED,
                 source);
  CheckCount();
}
