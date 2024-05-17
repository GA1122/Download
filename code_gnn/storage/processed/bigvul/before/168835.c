ExtensionFunction::ResponseAction WindowsCreateFunction::Run() {
  std::unique_ptr<windows::Create::Params> params(
      windows::Create::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params);
  std::vector<GURL> urls;
  TabStripModel* source_tab_strip = NULL;
  int tab_index = -1;

  windows::Create::Params::CreateData* create_data = params->create_data.get();

  if (create_data && create_data->url) {
    std::vector<std::string> url_strings;
    if (create_data->url->as_string)
      url_strings.push_back(*create_data->url->as_string);
    else if (create_data->url->as_strings)
      url_strings.swap(*create_data->url->as_strings);

    for (std::vector<std::string>::iterator i = url_strings.begin();
         i != url_strings.end(); ++i) {
      GURL url = ExtensionTabUtil::ResolvePossiblyRelativeURL(*i, extension());
      if (!url.is_valid())
        return RespondNow(Error(keys::kInvalidUrlError, *i));
      if (ExtensionTabUtil::IsKillURL(url))
        return RespondNow(Error(keys::kNoCrashBrowserError));
      urls.push_back(url);
    }
  }

  std::string error;
  bool open_incognito_window =
      ShouldOpenIncognitoWindow(create_data, &urls, &error);
  if (!error.empty())
    return RespondNow(Error(error));

  Profile* calling_profile = Profile::FromBrowserContext(browser_context());
  Profile* window_profile = open_incognito_window
                                ? calling_profile->GetOffTheRecordProfile()
                                : calling_profile;

  if (create_data && create_data->tab_id) {
    Browser* source_browser = nullptr;
    if (!GetTabById(*create_data->tab_id, calling_profile, include_incognito(),
                    &source_browser, &source_tab_strip, nullptr, &tab_index,
                    &error)) {
      return RespondNow(Error(error));
    }

    if (!source_browser->window()->IsTabStripEditable())
      return RespondNow(Error(keys::kTabStripNotEditableError));

    if (source_browser->profile() != window_profile)
      return RespondNow(Error(keys::kCanOnlyMoveTabsWithinSameProfileError));
  }

  if (!IsValidStateForWindowsCreateFunction(create_data))
    return RespondNow(Error(keys::kInvalidWindowStateError));

  Browser::Type window_type = Browser::TYPE_TABBED;

  gfx::Rect window_bounds;
  bool focused = true;
  std::string extension_id;

  if (create_data) {
    ReportRequestedWindowState(create_data->state);

    switch (create_data->type) {
      case windows::CREATE_TYPE_POPUP:
        window_type = Browser::TYPE_POPUP;
        extension_id = extension()->id();
        break;

      case windows::CREATE_TYPE_PANEL: {
        extension_id = extension()->id();
        window_type = Browser::TYPE_POPUP;
        break;
      }

      case windows::CREATE_TYPE_NONE:
      case windows::CREATE_TYPE_NORMAL:
        break;
      default:
        return RespondNow(Error(keys::kInvalidWindowTypeError));
    }

    if (window_type == Browser::TYPE_TABBED ||
        window_type == Browser::TYPE_POPUP) {
      ui::WindowShowState ignored_show_state = ui::SHOW_STATE_DEFAULT;
      WindowSizer::GetBrowserWindowBoundsAndShowState(
          std::string(), gfx::Rect(), nullptr, &window_bounds,
          &ignored_show_state);
    }

    if (create_data->left)
      window_bounds.set_x(*create_data->left);

    if (create_data->top)
      window_bounds.set_y(*create_data->top);

    if (create_data->width)
      window_bounds.set_width(*create_data->width);

    if (create_data->height)
      window_bounds.set_height(*create_data->height);

    if (create_data->focused)
      focused = *create_data->focused;
  }

  Browser::CreateParams create_params(window_type, window_profile,
                                      user_gesture());
  if (extension_id.empty()) {
    create_params.initial_bounds = window_bounds;
  } else {
    create_params = Browser::CreateParams::CreateForApp(
        web_app::GenerateApplicationNameFromExtensionId(extension_id),
        false  , window_bounds, window_profile,
        user_gesture());
  }
  create_params.initial_show_state = ui::SHOW_STATE_NORMAL;
  if (create_data && create_data->state) {
    if (create_data->state == windows::WINDOW_STATE_LOCKED_FULLSCREEN &&
        !ExtensionHasLockedFullscreenPermission(extension())) {
      return RespondNow(
          Error(keys::kMissingLockWindowFullscreenPrivatePermission));
    }
    create_params.initial_show_state =
        ConvertToWindowShowState(create_data->state);
  }

  Browser* new_window = new Browser(create_params);

  for (const GURL& url : urls) {
    NavigateParams navigate_params(new_window, url, ui::PAGE_TRANSITION_LINK);
    navigate_params.disposition = WindowOpenDisposition::NEW_FOREGROUND_TAB;

    bool set_self_as_opener = create_data->set_self_as_opener &&   
                              *create_data->set_self_as_opener;   
    navigate_params.opener = set_self_as_opener ? render_frame_host() : nullptr;
    navigate_params.source_site_instance =
        render_frame_host()->GetSiteInstance();

    Navigate(&navigate_params);
  }

  WebContents* contents = NULL;
  if ((window_type == Browser::TYPE_POPUP && urls.empty()) ||
      window_type == Browser::TYPE_TABBED) {
    if (source_tab_strip)
      contents = source_tab_strip->DetachWebContentsAt(tab_index);
    if (contents) {
      TabStripModel* target_tab_strip = new_window->tab_strip_model();
      target_tab_strip->InsertWebContentsAt(urls.size(), contents,
                                            TabStripModel::ADD_NONE);
    }
  }
  if (!contents && urls.empty() && window_type != Browser::TYPE_POPUP) {
    chrome::NewTab(new_window);
  }
  chrome::SelectNumberedTab(new_window, 0);

#if defined(OS_CHROMEOS)
  if (create_data &&
      create_data->state == windows::WINDOW_STATE_LOCKED_FULLSCREEN) {
    SetLockedFullscreenState(new_window, true);
  }
#endif

  if (focused)
    new_window->window()->Show();
  else
    new_window->window()->ShowInactive();

  std::unique_ptr<base::Value> result;
  if (new_window->profile()->IsOffTheRecord() &&
      !browser_context()->IsOffTheRecord() && !include_incognito()) {
    result = std::make_unique<base::Value>();
  } else {
    result = ExtensionTabUtil::CreateWindowValueForExtension(
        *new_window, extension(), ExtensionTabUtil::kPopulateTabs);
  }

  return RespondNow(OneArgument(std::move(result)));
}
