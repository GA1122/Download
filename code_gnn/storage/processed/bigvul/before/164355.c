ExtensionFunction::ResponseAction TabsDetectLanguageFunction::Run() {
  std::unique_ptr<tabs::DetectLanguage::Params> params(
      tabs::DetectLanguage::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  int tab_id = 0;
  Browser* browser = NULL;
  WebContents* contents = NULL;

  std::string error;
  if (params->tab_id.get()) {
    tab_id = *params->tab_id;
    if (!GetTabById(tab_id, browser_context(), include_incognito_information(),
                    &browser, nullptr, &contents, nullptr, &error)) {
      return RespondNow(Error(error));
    }
    if (!browser || !contents)
      return RespondNow(Error(kUnknownErrorDoNotUse));
  } else {
    browser = ChromeExtensionFunctionDetails(this).GetCurrentBrowser();
    if (!browser)
      return RespondNow(Error(tabs_constants::kNoCurrentWindowError));
    contents = browser->tab_strip_model()->GetActiveWebContents();
    if (!contents)
      return RespondNow(Error(tabs_constants::kNoSelectedTabError));
  }

  if (contents->GetController().NeedsReload()) {
    return RespondNow(
        Error(tabs_constants::kCannotDetermineLanguageOfUnloadedTab));
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
    return RespondLater();
  }
  registrar_.Add(this, chrome::NOTIFICATION_TAB_LANGUAGE_DETERMINED,
                 content::Source<WebContents>(contents));
  registrar_.Add(
      this, chrome::NOTIFICATION_TAB_CLOSING,
      content::Source<NavigationController>(&(contents->GetController())));
  registrar_.Add(
      this, content::NOTIFICATION_NAV_ENTRY_COMMITTED,
      content::Source<NavigationController>(&(contents->GetController())));
  return RespondLater();
}
