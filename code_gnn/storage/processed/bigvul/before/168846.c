bool TabsDetectLanguageFunction::RunAsync() {
  std::unique_ptr<tabs::DetectLanguage::Params> params(
      tabs::DetectLanguage::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  int tab_id = 0;
  Browser* browser = NULL;
  WebContents* contents = NULL;

  if (params->tab_id.get()) {
    tab_id = *params->tab_id;
    if (!GetTabById(tab_id, browser_context(), include_incognito(), &browser,
                    nullptr, &contents, nullptr, &error_)) {
      return false;
    }
    if (!browser || !contents)
      return false;
  } else {
    browser = ChromeExtensionFunctionDetails(this).GetCurrentBrowser();
    if (!browser)
      return false;
    contents = browser->tab_strip_model()->GetActiveWebContents();
    if (!contents)
      return false;
  }

  if (contents->GetController().NeedsReload()) {
    error_ = keys::kCannotDetermineLanguageOfUnloadedTab;
    return false;
  }

  AddRef();   

  ChromeTranslateClient* chrome_translate_client =
      ChromeTranslateClient::FromWebContents(contents);
  if (!chrome_translate_client->GetLanguageState()
           .original_language()
           .empty()) {
    base::ThreadTaskRunnerHandle::Get()->PostTask(
        FROM_HERE,
        base::BindOnce(
            &TabsDetectLanguageFunction::GotLanguage, this,
            chrome_translate_client->GetLanguageState().original_language()));
    return true;
  }
  registrar_.Add(this, chrome::NOTIFICATION_TAB_LANGUAGE_DETERMINED,
                 content::Source<WebContents>(contents));
  registrar_.Add(
      this, chrome::NOTIFICATION_TAB_CLOSING,
      content::Source<NavigationController>(&(contents->GetController())));
  registrar_.Add(
      this, content::NOTIFICATION_NAV_ENTRY_COMMITTED,
      content::Source<NavigationController>(&(contents->GetController())));
  return true;
}
