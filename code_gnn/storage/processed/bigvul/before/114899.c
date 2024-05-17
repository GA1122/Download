void TestingAutomationProvider::GetBrowserInfo(
    DictionaryValue* args,
    IPC::Message* reply_message) {
  base::ThreadRestrictions::ScopedAllowIO allow_io;   
  DictionaryValue* properties = new DictionaryValue;
  properties->SetString("ChromeVersion", chrome::kChromeVersion);
  properties->SetString("BrowserProcessExecutableName",
                        chrome::kBrowserProcessExecutableName);
  properties->SetString("HelperProcessExecutableName",
                        chrome::kHelperProcessExecutableName);
  properties->SetString("BrowserProcessExecutablePath",
                        chrome::kBrowserProcessExecutablePath);
  properties->SetString("HelperProcessExecutablePath",
                        chrome::kHelperProcessExecutablePath);
  properties->SetString("command_line_string",
      CommandLine::ForCurrentProcess()->GetCommandLineString());
  FilePath dumps_path;
  PathService::Get(chrome::DIR_CRASH_DUMPS, &dumps_path);
  properties->SetString("DIR_CRASH_DUMPS", dumps_path.value());

  std::string branding;
#if defined(GOOGLE_CHROME_BUILD)
  branding = "Google Chrome";
#elif defined(CHROMIUM_BUILD)
  branding = "Chromium";
#else
  branding = "Unknown Branding";
#endif
  properties->SetString("branding", branding);

  bool is_official_build = false;
#if defined(OFFICIAL_BUILD)
  is_official_build = true;
#endif
  properties->SetBoolean("is_official", is_official_build);

  scoped_ptr<DictionaryValue> return_value(new DictionaryValue);
  return_value->Set("properties", properties);

  return_value->SetInteger("browser_pid", base::GetCurrentProcId());
  ListValue* windows = new ListValue;
  int windex = 0;
  for (BrowserList::const_iterator it = BrowserList::begin();
       it != BrowserList::end();
       ++it, ++windex) {
    DictionaryValue* browser_item = new DictionaryValue;
    Browser* browser = *it;
    browser_item->SetInteger("index", windex);
    gfx::Rect rect = browser->window()->GetRestoredBounds();
    browser_item->SetInteger("x", rect.x());
    browser_item->SetInteger("y", rect.y());
    browser_item->SetInteger("width", rect.width());
    browser_item->SetInteger("height", rect.height());
    browser_item->SetBoolean("fullscreen",
                             browser->window()->IsFullscreen());
    ListValue* visible_page_actions = new ListValue;
    LocationBarTesting* loc_bar =
        browser->window()->GetLocationBar()->GetLocationBarForTesting();
    size_t page_action_visible_count =
        static_cast<size_t>(loc_bar->PageActionVisibleCount());
    for (size_t i = 0; i < page_action_visible_count; ++i) {
      StringValue* extension_id = new StringValue(
          loc_bar->GetVisiblePageAction(i)->extension_id());
      visible_page_actions->Append(extension_id);
    }
    browser_item->Set("visible_page_actions", visible_page_actions);
    browser_item->SetInteger("selected_tab", browser->active_index());
    browser_item->SetBoolean("incognito",
                             browser->profile()->IsOffTheRecord());
    browser_item->SetString("profile_path",
        browser->profile()->GetPath().BaseName().MaybeAsASCII());
    std::string type;
    switch (browser->type()) {
      case Browser::TYPE_TABBED:
        type = "tabbed";
        break;
      case Browser::TYPE_POPUP:
        type = "popup";
        break;
      case Browser::TYPE_PANEL:
        type = "panel";
        break;
      default:
        type = "unknown";
        break;
    }
    browser_item->SetString("type", type);
    ListValue* tabs = new ListValue;
    for (int i = 0; i < browser->tab_count(); ++i) {
      WebContents* wc = browser->GetWebContentsAt(i);
      DictionaryValue* tab = new DictionaryValue;
      tab->SetInteger("index", i);
      tab->SetString("url", wc->GetURL().spec());
      tab->SetInteger("renderer_pid",
                      base::GetProcId(wc->GetRenderProcessHost()->GetHandle()));
      tab->Set("infobars", GetInfobarsInfo(wc));
      tab->SetBoolean("pinned", browser->IsTabPinned(i));
      tabs->Append(tab);
    }
    browser_item->Set("tabs", tabs);

    windows->Append(browser_item);
  }
  return_value->Set("windows", windows);

#if defined(OS_LINUX)
  int flags = ChildProcessHost::CHILD_ALLOW_SELF;
#else
  int flags = ChildProcessHost::CHILD_NORMAL;
#endif

  ListValue* extension_views = new ListValue;
  ProfileManager* profile_manager = g_browser_process->profile_manager();
  std::vector<Profile*> profiles(profile_manager->GetLoadedProfiles());
  for (size_t i = 0; i < profiles.size(); ++i) {
    ExtensionProcessManager* process_manager =
        profiles[i]->GetExtensionProcessManager();
    if (!process_manager)
      continue;
    ExtensionProcessManager::const_iterator jt;
    for (jt = process_manager->begin(); jt != process_manager->end(); ++jt) {
      ExtensionHost* ex_host = *jt;
      if (!ex_host->IsRenderViewLive())
        continue;
      DictionaryValue* item = new DictionaryValue;
      item->SetString("name", ex_host->extension()->name());
      item->SetString("extension_id", ex_host->extension()->id());
      item->SetInteger(
          "pid",
          base::GetProcId(ex_host->render_process_host()->GetHandle()));
      DictionaryValue* view = new DictionaryValue;
      view->SetInteger(
          "render_process_id",
          ex_host->render_process_host()->GetID());
      view->SetInteger(
          "render_view_id",
          ex_host->render_view_host()->routing_id());
      item->Set("view", view);
      std::string type;
      switch (ex_host->extension_host_type()) {
        case chrome::VIEW_TYPE_EXTENSION_BACKGROUND_PAGE:
          type = "EXTENSION_BACKGROUND_PAGE";
          break;
        case chrome::VIEW_TYPE_EXTENSION_POPUP:
          type = "EXTENSION_POPUP";
          break;
        case chrome::VIEW_TYPE_EXTENSION_INFOBAR:
          type = "EXTENSION_INFOBAR";
          break;
        case chrome::VIEW_TYPE_EXTENSION_DIALOG:
          type = "EXTENSION_DIALOG";
          break;
        case chrome::VIEW_TYPE_APP_SHELL:
          type = "APP_SHELL";
          break;
        default:
          type = "unknown";
          break;
      }
      item->SetString("view_type", type);
      item->SetString("url", ex_host->GetURL().spec());
      item->SetBoolean("loaded", ex_host->did_stop_loading());
      extension_views->Append(item);
    }
  }
  return_value->Set("extension_views", extension_views);

  return_value->SetString("child_process_path",
                          ChildProcessHost::GetChildPath(flags).value());
  ListValue* child_processes = new ListValue;
  return_value->Set("child_processes", child_processes);
  BrowserThread::PostTaskAndReply(
      BrowserThread::IO, FROM_HERE,
      base::Bind(&GetChildProcessHostInfo, child_processes),
      base::Bind(&AutomationJSONReply::SendSuccess,
                 base::Owned(new AutomationJSONReply(this, reply_message)),
                 base::Owned(return_value.release())));
}
