void BrowserCommandController::ExecuteCommandWithDisposition(
  int id, WindowOpenDisposition disposition) {
  if (!chrome::GetActiveTabContents(browser_))
    return;

  DCHECK(command_updater_.IsCommandEnabled(id)) << "Invalid/disabled command "
                                                << id;

  if (block_command_execution_) {
    DCHECK_EQ(last_blocked_command_id_, -1);
    last_blocked_command_id_ = id;
    last_blocked_command_disposition_ = disposition;
    return;
  }

  switch (id) {
    case IDC_BACK:
      GoBack(browser_, disposition);
      break;
    case IDC_FORWARD:
      GoForward(browser_, disposition);
      break;
    case IDC_RELOAD:
      Reload(browser_, disposition);
      break;
    case IDC_RELOAD_CLEARING_CACHE:
      ClearCache(browser_);
    case IDC_RELOAD_IGNORING_CACHE:
      ReloadIgnoringCache(browser_, disposition);
      break;
    case IDC_HOME:
      Home(browser_, disposition);
      break;
    case IDC_OPEN_CURRENT_URL:
      OpenCurrentURL(browser_);
      break;
    case IDC_STOP:
      Stop(browser_);
      break;

    case IDC_NEW_WINDOW:
      NewWindow(browser_);
      break;
    case IDC_NEW_INCOGNITO_WINDOW:
      NewIncognitoWindow(browser_);
      break;
    case IDC_CLOSE_WINDOW:
      CloseWindow(browser_);
      break;
    case IDC_NEW_TAB:
      NewTab(browser_);
      break;
    case IDC_CLOSE_TAB:
      CloseTab(browser_);
      break;
    case IDC_SELECT_NEXT_TAB:
      SelectNextTab(browser_);
      break;
    case IDC_SELECT_PREVIOUS_TAB:
      SelectPreviousTab(browser_);
      break;
    case IDC_TABPOSE:
      OpenTabpose(browser_);
      break;
    case IDC_MOVE_TAB_NEXT:
      MoveTabNext(browser_);
      break;
    case IDC_MOVE_TAB_PREVIOUS:
      MoveTabPrevious(browser_);
      break;
    case IDC_SELECT_TAB_0:
    case IDC_SELECT_TAB_1:
    case IDC_SELECT_TAB_2:
    case IDC_SELECT_TAB_3:
    case IDC_SELECT_TAB_4:
    case IDC_SELECT_TAB_5:
    case IDC_SELECT_TAB_6:
    case IDC_SELECT_TAB_7:
      SelectNumberedTab(browser_, id - IDC_SELECT_TAB_0);
      break;
    case IDC_SELECT_LAST_TAB:
      SelectLastTab(browser_);
      break;
    case IDC_DUPLICATE_TAB:
      DuplicateTab(browser_);
      break;
    case IDC_RESTORE_TAB:
      RestoreTab(browser_);
      break;
    case IDC_SHOW_AS_TAB:
      ConvertPopupToTabbedBrowser(browser_);
      break;
    case IDC_FULLSCREEN:
      chrome::ToggleFullscreenMode(browser_);
      break;

#if defined(USE_ASH)
    case IDC_TOGGLE_ASH_DESKTOP:
      chrome::ToggleAshDesktop();
      break;
#endif

#if defined(OS_WIN)
    case IDC_METRO_SNAP_ENABLE:
      browser_->SetMetroSnapMode(true);
      break;
    case IDC_METRO_SNAP_DISABLE:
      browser_->SetMetroSnapMode(false);
      break;
    case IDC_WIN8_DESKTOP_RESTART:
      browser::AttemptRestartWithModeSwitch();
      content::RecordAction(content::UserMetricsAction("Win8DesktopRestart"));
      break;
    case IDC_WIN8_METRO_RESTART:
      new SwichToMetroUIHandler;
      content::RecordAction(content::UserMetricsAction("Win8MetroRestart"));
      break;
#endif

#if defined(OS_MACOSX)
    case IDC_PRESENTATION_MODE:
      browser_->TogglePresentationMode();
      break;
#endif
    case IDC_EXIT:
      Exit();
      break;

    case IDC_SAVE_PAGE:
      SavePage(browser_);
      break;
    case IDC_BOOKMARK_PAGE:
      BookmarkCurrentPage(browser_);
      break;
    case IDC_BOOKMARK_PAGE_FROM_STAR:
      BookmarkCurrentPageFromStar(browser_);
      break;
    case IDC_PIN_TO_START_SCREEN:
      TogglePagePinnedToStartScreen(browser_);
      break;
    case IDC_BOOKMARK_ALL_TABS:
      BookmarkAllTabs(browser_);
      break;
    case IDC_VIEW_SOURCE:
      ViewSelectedSource(browser_);
      break;
    case IDC_EMAIL_PAGE_LOCATION:
      EmailPageLocation(browser_);
      break;
    case IDC_PRINT:
      Print(browser_);
      break;
    case IDC_ADVANCED_PRINT:
      AdvancedPrint(browser_);
      break;
    case IDC_PRINT_TO_DESTINATION:
      PrintToDestination(browser_);
      break;
    case IDC_CHROME_TO_MOBILE_PAGE:
      ShowChromeToMobileBubble(browser_);
      break;
    case IDC_ENCODING_AUTO_DETECT:
      browser_->ToggleEncodingAutoDetect();
      break;
    case IDC_ENCODING_UTF8:
    case IDC_ENCODING_UTF16LE:
    case IDC_ENCODING_ISO88591:
    case IDC_ENCODING_WINDOWS1252:
    case IDC_ENCODING_GBK:
    case IDC_ENCODING_GB18030:
    case IDC_ENCODING_BIG5HKSCS:
    case IDC_ENCODING_BIG5:
    case IDC_ENCODING_KOREAN:
    case IDC_ENCODING_SHIFTJIS:
    case IDC_ENCODING_ISO2022JP:
    case IDC_ENCODING_EUCJP:
    case IDC_ENCODING_THAI:
    case IDC_ENCODING_ISO885915:
    case IDC_ENCODING_MACINTOSH:
    case IDC_ENCODING_ISO88592:
    case IDC_ENCODING_WINDOWS1250:
    case IDC_ENCODING_ISO88595:
    case IDC_ENCODING_WINDOWS1251:
    case IDC_ENCODING_KOI8R:
    case IDC_ENCODING_KOI8U:
    case IDC_ENCODING_ISO88597:
    case IDC_ENCODING_WINDOWS1253:
    case IDC_ENCODING_ISO88594:
    case IDC_ENCODING_ISO885913:
    case IDC_ENCODING_WINDOWS1257:
    case IDC_ENCODING_ISO88593:
    case IDC_ENCODING_ISO885910:
    case IDC_ENCODING_ISO885914:
    case IDC_ENCODING_ISO885916:
    case IDC_ENCODING_WINDOWS1254:
    case IDC_ENCODING_ISO88596:
    case IDC_ENCODING_WINDOWS1256:
    case IDC_ENCODING_ISO88598:
    case IDC_ENCODING_ISO88598I:
    case IDC_ENCODING_WINDOWS1255:
    case IDC_ENCODING_WINDOWS1258:
      browser_->OverrideEncoding(id);
      break;

    case IDC_CUT:
      Cut(browser_);
      break;
    case IDC_COPY:
      Copy(browser_);
      break;
    case IDC_PASTE:
      Paste(browser_);
      break;

    case IDC_FIND:
      Find(browser_);
      break;
    case IDC_FIND_NEXT:
      FindNext(browser_);
      break;
    case IDC_FIND_PREVIOUS:
      FindPrevious(browser_);
      break;

    case IDC_ZOOM_PLUS:
      Zoom(browser_, content::PAGE_ZOOM_IN);
      break;
    case IDC_ZOOM_NORMAL:
      Zoom(browser_, content::PAGE_ZOOM_RESET);
      break;
    case IDC_ZOOM_MINUS:
      Zoom(browser_, content::PAGE_ZOOM_OUT);
      break;

    case IDC_FOCUS_TOOLBAR:
      FocusToolbar(browser_);
      break;
    case IDC_FOCUS_LOCATION:
      FocusLocationBar(browser_);
      break;
    case IDC_FOCUS_SEARCH:
      FocusSearch(browser_);
      break;
    case IDC_FOCUS_MENU_BAR:
      FocusAppMenu(browser_);
      break;
    case IDC_FOCUS_BOOKMARKS:
      FocusBookmarksToolbar(browser_);
      break;
    case IDC_FOCUS_NEXT_PANE:
      FocusNextPane(browser_);
      break;
    case IDC_FOCUS_PREVIOUS_PANE:
      FocusPreviousPane(browser_);
      break;

    case IDC_OPEN_FILE:
      browser_->OpenFile();
      break;
    case IDC_CREATE_SHORTCUTS:
      CreateApplicationShortcuts(browser_);
      break;
    case IDC_DEV_TOOLS:
      ToggleDevToolsWindow(browser_, DEVTOOLS_TOGGLE_ACTION_SHOW);
      break;
    case IDC_DEV_TOOLS_CONSOLE:
      ToggleDevToolsWindow(browser_, DEVTOOLS_TOGGLE_ACTION_SHOW_CONSOLE);
      break;
    case IDC_DEV_TOOLS_INSPECT:
      ToggleDevToolsWindow(browser_, DEVTOOLS_TOGGLE_ACTION_INSPECT);
      break;
    case IDC_DEV_TOOLS_TOGGLE:
      ToggleDevToolsWindow(browser_, DEVTOOLS_TOGGLE_ACTION_TOGGLE);
      break;
    case IDC_TASK_MANAGER:
      OpenTaskManager(browser_, false);
      break;
    case IDC_VIEW_BACKGROUND_PAGES:
      OpenTaskManager(browser_, true);
      break;
    case IDC_FEEDBACK:
      OpenFeedbackDialog(browser_);
      break;

    case IDC_SHOW_BOOKMARK_BAR:
      ToggleBookmarkBar(browser_);
      break;
    case IDC_PROFILING_ENABLED:
      Profiling::Toggle();
      break;

    case IDC_SHOW_BOOKMARK_MANAGER:
      ShowBookmarkManager(browser_);
      break;
    case IDC_SHOW_APP_MENU:
      ShowAppMenu(browser_);
      break;
    case IDC_SHOW_AVATAR_MENU:
      ShowAvatarMenu(browser_);
      break;
    case IDC_SHOW_HISTORY:
      ShowHistory(browser_);
      break;
    case IDC_SHOW_DOWNLOADS:
      ShowDownloads(browser_);
      break;
    case IDC_MANAGE_EXTENSIONS:
      ShowExtensions(browser_);
      break;
    case IDC_OPTIONS:
      ShowSettings(browser_);
      break;
    case IDC_EDIT_SEARCH_ENGINES:
      ShowSearchEngineSettings(browser_);
      break;
    case IDC_VIEW_PASSWORDS:
      ShowPasswordManager(browser_);
      break;
    case IDC_CLEAR_BROWSING_DATA:
      ShowClearBrowsingDataDialog(browser_);
      break;
    case IDC_IMPORT_SETTINGS:
      ShowImportDialog(browser_);
      break;
    case IDC_TOGGLE_REQUEST_TABLET_SITE:
      ToggleRequestTabletSite(browser_);
      break;
    case IDC_ABOUT:
      ShowAboutChrome(browser_);
      break;
    case IDC_UPGRADE_DIALOG:
      OpenUpdateChromeDialog(browser_);
      break;
    case IDC_VIEW_INCOMPATIBILITIES:
      ShowConflicts(browser_);
      break;
    case IDC_HELP_PAGE_VIA_KEYBOARD:
      ShowHelp(browser_, HELP_SOURCE_KEYBOARD);
      break;
    case IDC_HELP_PAGE_VIA_MENU:
      ShowHelp(browser_, HELP_SOURCE_MENU);
      break;
    case IDC_SHOW_SYNC_SETUP:
      ShowSyncSetup(browser_, SyncPromoUI::SOURCE_MENU);
      break;
    case IDC_TOGGLE_SPEECH_INPUT:
      ToggleSpeechInput(browser_);
      break;

    default:
      LOG(WARNING) << "Received Unimplemented Command: " << id;
      break;
  }
}
