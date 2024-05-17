void TestingAutomationProvider::GetTranslateInfo(
    Browser* browser,
    DictionaryValue* args,
    IPC::Message* reply_message) {
  std::string error_message;
  TabContentsWrapper* tab_contents_wrapper =
      GetTabContentsWrapperFromDict(browser, args, &error_message);
  if (!tab_contents_wrapper) {
    AutomationJSONReply(this, reply_message).SendError(error_message);
    return;
  }

  WebContents* web_contents = tab_contents_wrapper->web_contents();
  TranslateInfoBarDelegate* translate_bar =
      GetTranslateInfoBarDelegate(web_contents);

  TabLanguageDeterminedObserver* observer = new TabLanguageDeterminedObserver(
      this, reply_message, web_contents, translate_bar);
  TranslateTabHelper* helper = TabContentsWrapper::GetCurrentWrapperForContents(
      web_contents)->translate_tab_helper();
  std::string language = helper->language_state().original_language();
  if (!language.empty()) {
    observer->Observe(chrome::NOTIFICATION_TAB_LANGUAGE_DETERMINED,
                      content::Source<WebContents>(web_contents),
                      content::Details<std::string>(&language));
  }
}
