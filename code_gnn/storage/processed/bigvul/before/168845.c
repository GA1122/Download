bool TabsUpdateFunction::RunAsync() {
  std::unique_ptr<tabs::Update::Params> params(
      tabs::Update::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  int tab_id = -1;
  WebContents* contents = NULL;
  if (!params->tab_id.get()) {
    Browser* browser = ChromeExtensionFunctionDetails(this).GetCurrentBrowser();
    if (!browser) {
      error_ = keys::kNoCurrentWindowError;
      return false;
    }
    contents = browser->tab_strip_model()->GetActiveWebContents();
    if (!contents) {
      error_ = keys::kNoSelectedTabError;
      return false;
    }
    tab_id = SessionTabHelper::IdForTab(contents).id();
  } else {
    tab_id = *params->tab_id;
  }

  int tab_index = -1;
  TabStripModel* tab_strip = NULL;
  Browser* browser = nullptr;
  if (!GetTabById(tab_id, browser_context(), include_incognito(), &browser,
                  &tab_strip, &contents, &tab_index, &error_)) {
    return false;
  }

  if (!ExtensionTabUtil::BrowserSupportsTabs(browser)) {
    error_ = keys::kNoCurrentWindowError;
    return false;
  }

  web_contents_ = contents;


  bool is_async = false;
  if (params->update_properties.url.get()) {
    std::string updated_url = *params->update_properties.url;
    if (browser->profile()->GetProfileType() == Profile::INCOGNITO_PROFILE &&
        !IsURLAllowedInIncognito(GURL(updated_url), browser->profile())) {
      error_ = ErrorUtils::FormatErrorMessage(
          keys::kURLsNotAllowedInIncognitoError, updated_url);
      return false;
    }
    if (!UpdateURL(updated_url, tab_id, &is_async))
      return false;
  }

  bool active = false;
  if (params->update_properties.selected.get())
    active = *params->update_properties.selected;

  if (params->update_properties.active.get())
    active = *params->update_properties.active;

  if (active) {
    if (tab_strip->active_index() != tab_index) {
      tab_strip->ActivateTabAt(tab_index, false);
      DCHECK_EQ(contents, tab_strip->GetActiveWebContents());
    }
  }

  if (params->update_properties.highlighted.get()) {
    bool highlighted = *params->update_properties.highlighted;
    if (highlighted != tab_strip->IsTabSelected(tab_index))
      tab_strip->ToggleSelectionAt(tab_index);
  }

  if (params->update_properties.pinned.get()) {
    bool pinned = *params->update_properties.pinned;
    tab_strip->SetTabPinned(tab_index, pinned);

    tab_index = tab_strip->GetIndexOfWebContents(contents);
  }

  if (params->update_properties.muted.get()) {
    TabMutedResult tab_muted_result = chrome::SetTabAudioMuted(
        contents, *params->update_properties.muted,
        TabMutedReason::EXTENSION, extension()->id());

    switch (tab_muted_result) {
      case TabMutedResult::SUCCESS:
        break;
      case TabMutedResult::FAIL_NOT_ENABLED:
        error_ = ErrorUtils::FormatErrorMessage(
            keys::kCannotUpdateMuteDisabled, base::IntToString(tab_id),
            switches::kEnableTabAudioMuting);
        return false;
      case TabMutedResult::FAIL_TABCAPTURE:
        error_ = ErrorUtils::FormatErrorMessage(keys::kCannotUpdateMuteCaptured,
                                                base::IntToString(tab_id));
        return false;
    }
  }

  if (params->update_properties.opener_tab_id.get()) {
    int opener_id = *params->update_properties.opener_tab_id;
    WebContents* opener_contents = NULL;
    if (opener_id == tab_id) {
      error_ = "Cannot set a tab's opener to itself.";
      return false;
    }
    if (!ExtensionTabUtil::GetTabById(opener_id, browser_context(),
                                      include_incognito(), nullptr, nullptr,
                                      &opener_contents, nullptr))
      return false;

    if (tab_strip->GetIndexOfWebContents(opener_contents) ==
        TabStripModel::kNoTab) {
      error_ = "Tab opener must be in the same window as the updated tab.";
      return false;
    }
    tab_strip->SetOpenerOfWebContentsAt(tab_index, opener_contents);
  }

  if (params->update_properties.auto_discardable.get()) {
    bool state = *params->update_properties.auto_discardable;
    g_browser_process->GetTabManager()->SetTabAutoDiscardableState(contents,
                                                                   state);
  }

  if (!is_async) {
    PopulateResult();
    SendResponse(true);
  }
  return true;
}
