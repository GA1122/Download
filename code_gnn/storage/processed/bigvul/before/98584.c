bool DetectTabLanguageFunction::RunImpl() {
  int tab_id = 0;
  Browser* browser = NULL;
  TabContents* contents = NULL;

  if (HasOptionalArgument(0)) {
    EXTENSION_FUNCTION_VALIDATE(args_->GetInteger(0, &tab_id));
    if (!GetTabById(tab_id, profile(), include_incognito(),
                    &browser, NULL, &contents, NULL, &error_)) {
      return false;
    }
    if (!browser || !contents)
      return false;
  } else {
    browser = GetCurrentBrowser();
    if (!browser)
      return false;
    contents = browser->tabstrip_model()->GetSelectedTabContents();
    if (!contents)
      return false;
  }

  if (contents->controller().needs_reload()) {
    error_ = keys::kCannotDetermineLanguageOfUnloadedTab;
    return false;
  }

  AddRef();   

  if (!contents->language_state().original_language().empty()) {
    MessageLoop::current()->PostTask(FROM_HERE, NewRunnableMethod(
        this, &DetectTabLanguageFunction::GotLanguage,
        contents->language_state().original_language()));
    return true;
  }
  registrar_.Add(this, NotificationType::TAB_LANGUAGE_DETERMINED,
                 Source<TabContents>(contents));
  registrar_.Add(this, NotificationType::TAB_CLOSING,
                 Source<NavigationController>(&(contents->controller())));
  registrar_.Add(this, NotificationType::NAV_ENTRY_COMMITTED,
                 Source<NavigationController>(&(contents->controller())));
  return true;
}
