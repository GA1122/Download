ExtensionFunction::ResponseAction TabsQueryFunction::Run() {
  std::unique_ptr<tabs::Query::Params> params(
      tabs::Query::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  bool loading_status_set = params->query_info.status != tabs::TAB_STATUS_NONE;
  bool loading = params->query_info.status == tabs::TAB_STATUS_LOADING;

  URLPatternSet url_patterns;
  if (params->query_info.url.get()) {
    std::vector<std::string> url_pattern_strings;
    if (params->query_info.url->as_string)
      url_pattern_strings.push_back(*params->query_info.url->as_string);
    else if (params->query_info.url->as_strings)
      url_pattern_strings.swap(*params->query_info.url->as_strings);
    std::string error;
    if (!url_patterns.Populate(url_pattern_strings, URLPattern::SCHEME_ALL,
                               true, &error)) {
      return RespondNow(Error(error));
    }
  }

  std::string title;
  if (params->query_info.title.get())
    title = *params->query_info.title;

  int window_id = extension_misc::kUnknownWindowId;
  if (params->query_info.window_id.get())
    window_id = *params->query_info.window_id;

  int index = -1;
  if (params->query_info.index.get())
    index = *params->query_info.index;

  std::string window_type;
  if (params->query_info.window_type != tabs::WINDOW_TYPE_NONE)
    window_type = tabs::ToString(params->query_info.window_type);

  std::unique_ptr<base::ListValue> result(new base::ListValue());
  Profile* profile = Profile::FromBrowserContext(browser_context());
  Browser* last_active_browser =
      chrome::FindAnyBrowser(profile, include_incognito());
  Browser* current_browser =
      ChromeExtensionFunctionDetails(this).GetCurrentBrowser();
  for (auto* browser : *BrowserList::GetInstance()) {
    if (!profile->IsSameProfile(browser->profile()))
      continue;

    if (!browser->window())
      continue;

    if (!include_incognito() && profile != browser->profile())
      continue;

    if (!browser->extension_window_controller()->IsVisibleToTabsAPIForExtension(
            extension(), false  )) {
      continue;
    }

    if (window_id >= 0 && window_id != ExtensionTabUtil::GetWindowId(browser))
      continue;

    if (window_id == extension_misc::kCurrentWindowId &&
        browser != current_browser) {
      continue;
    }

    if (!MatchesBool(params->query_info.current_window.get(),
                     browser == current_browser)) {
      continue;
    }

    if (!MatchesBool(params->query_info.last_focused_window.get(),
                     browser == last_active_browser)) {
      continue;
    }

    if (!window_type.empty() &&
        window_type !=
            browser->extension_window_controller()->GetWindowTypeText()) {
      continue;
    }

    TabStripModel* tab_strip = browser->tab_strip_model();
    for (int i = 0; i < tab_strip->count(); ++i) {
      WebContents* web_contents = tab_strip->GetWebContentsAt(i);

      if (index > -1 && i != index)
        continue;

      if (!web_contents) {
        continue;
      }

      if (!MatchesBool(params->query_info.highlighted.get(),
                       tab_strip->IsTabSelected(i))) {
        continue;
      }

      if (!MatchesBool(params->query_info.active.get(),
                       i == tab_strip->active_index())) {
        continue;
      }

      if (!MatchesBool(params->query_info.pinned.get(),
                       tab_strip->IsTabPinned(i))) {
        continue;
      }

      if (!MatchesBool(params->query_info.audible.get(),
                       web_contents->WasRecentlyAudible())) {
        continue;
      }

      auto* tab_lifecycle_unit_external =
          resource_coordinator::TabLifecycleUnitExternal::FromWebContents(
              web_contents);

      if (!MatchesBool(params->query_info.discarded.get(),
                       tab_lifecycle_unit_external->IsDiscarded())) {
        continue;
      }

      if (!MatchesBool(params->query_info.auto_discardable.get(),
                       tab_lifecycle_unit_external->IsAutoDiscardable())) {
        continue;
      }

      if (!MatchesBool(params->query_info.muted.get(),
                       web_contents->IsAudioMuted())) {
        continue;
      }

      if (!title.empty() || !url_patterns.is_empty()) {
        if (!extension_->permissions_data()->HasAPIPermissionForTab(
                ExtensionTabUtil::GetTabId(web_contents),
                APIPermission::kTab) &&
            !extension_->permissions_data()->HasHostPermission(
                web_contents->GetURL())) {
          continue;
        }

        if (!title.empty() &&
            !base::MatchPattern(web_contents->GetTitle(),
                                base::UTF8ToUTF16(title))) {
          continue;
        }

        if (!url_patterns.is_empty() &&
            !url_patterns.MatchesURL(web_contents->GetURL())) {
          continue;
        }
      }

      if (loading_status_set && loading != web_contents->IsLoading())
        continue;

      result->Append(ExtensionTabUtil::CreateTabObject(
                         web_contents, ExtensionTabUtil::kScrubTab, extension(),
                         tab_strip, i)
                         ->ToValue());
    }
  }

  return RespondNow(OneArgument(std::move(result)));
}
