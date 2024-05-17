void Browser::ExecuteCommandWithDisposition(
  int id, WindowOpenDisposition disposition) {
  if (!GetSelectedTabContentsWrapper())
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
    case IDC_BACK:                  GoBack(disposition);              break;
    case IDC_FORWARD:               GoForward(disposition);           break;
    case IDC_RELOAD:                Reload(disposition);              break;
    case IDC_RELOAD_IGNORING_CACHE: ReloadIgnoringCache(disposition); break;
    case IDC_HOME:                  Home(disposition);                break;
    case IDC_OPEN_CURRENT_URL:      OpenCurrentURL();                 break;
    case IDC_STOP:                  Stop();                           break;

    case IDC_NEW_WINDOW:            NewWindow();                      break;
    case IDC_NEW_INCOGNITO_WINDOW:  NewIncognitoWindow();             break;
    case IDC_CLOSE_WINDOW:          CloseWindow();                    break;
    case IDC_NEW_TAB:               NewTab();                         break;
    case IDC_CLOSE_TAB:             CloseTab();                       break;
    case IDC_SELECT_NEXT_TAB:       SelectNextTab();                  break;
    case IDC_SELECT_PREVIOUS_TAB:   SelectPreviousTab();              break;
    case IDC_TABPOSE:               OpenTabpose();                    break;
    case IDC_MOVE_TAB_NEXT:         MoveTabNext();                    break;
    case IDC_MOVE_TAB_PREVIOUS:     MoveTabPrevious();                break;
    case IDC_SELECT_TAB_0:
    case IDC_SELECT_TAB_1:
    case IDC_SELECT_TAB_2:
    case IDC_SELECT_TAB_3:
    case IDC_SELECT_TAB_4:
    case IDC_SELECT_TAB_5:
    case IDC_SELECT_TAB_6:
    case IDC_SELECT_TAB_7:          SelectNumberedTab(id - IDC_SELECT_TAB_0);
                                                                      break;
    case IDC_SELECT_LAST_TAB:       SelectLastTab();                  break;
    case IDC_DUPLICATE_TAB:         DuplicateTab();                   break;
    case IDC_RESTORE_TAB:           RestoreTab();                     break;
    case IDC_COPY_URL:              WriteCurrentURLToClipboard();     break;
    case IDC_SHOW_AS_TAB:           ConvertPopupToTabbedBrowser();    break;
    case IDC_FULLSCREEN:            ToggleFullscreenMode();           break;
#if defined(OS_MACOSX)
    case IDC_PRESENTATION_MODE:     TogglePresentationMode();         break;
#endif
    case IDC_EXIT:                  Exit();                           break;
    case IDC_TOGGLE_VERTICAL_TABS:  ToggleUseVerticalTabs();          break;
    case IDC_COMPACT_NAVBAR:        ToggleUseCompactNavigationBar();  break;
#if defined(OS_CHROMEOS)
    case IDC_SEARCH:                Search();                         break;
    case IDC_SHOW_KEYBOARD_OVERLAY: ShowKeyboardOverlay();            break;
#endif

    case IDC_SAVE_PAGE:             SavePage();                       break;
    case IDC_BOOKMARK_PAGE:         BookmarkCurrentPage();            break;
    case IDC_BOOKMARK_ALL_TABS:     BookmarkAllTabs();                break;
    case IDC_VIEW_SOURCE:           ViewSelectedSource();             break;
    case IDC_EMAIL_PAGE_LOCATION:   EmailPageLocation();              break;
    case IDC_PRINT:                 Print();                          break;
    case IDC_ADVANCED_PRINT:        AdvancedPrint();                  break;
    case IDC_ENCODING_AUTO_DETECT:  ToggleEncodingAutoDetect();       break;
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
    case IDC_ENCODING_WINDOWS1258:  OverrideEncoding(id);             break;

    case IDC_CUT:                   Cut();                            break;
    case IDC_COPY:                  Copy();                           break;
    case IDC_PASTE:                 Paste();                          break;

    case IDC_FIND:                  Find();                           break;
    case IDC_FIND_NEXT:             FindNext();                       break;
    case IDC_FIND_PREVIOUS:         FindPrevious();                   break;

    case IDC_ZOOM_PLUS:             Zoom(PageZoom::ZOOM_IN);          break;
    case IDC_ZOOM_NORMAL:           Zoom(PageZoom::RESET);            break;
    case IDC_ZOOM_MINUS:            Zoom(PageZoom::ZOOM_OUT);         break;

    case IDC_FOCUS_TOOLBAR:         FocusToolbar();                   break;
    case IDC_FOCUS_LOCATION:        FocusLocationBar();               break;
    case IDC_FOCUS_SEARCH:          FocusSearch();                    break;
    case IDC_FOCUS_MENU_BAR:        FocusAppMenu();                   break;
    case IDC_FOCUS_BOOKMARKS:       FocusBookmarksToolbar();          break;
    case IDC_FOCUS_CHROMEOS_STATUS: FocusChromeOSStatus();            break;
    case IDC_FOCUS_NEXT_PANE:       FocusNextPane();                  break;
    case IDC_FOCUS_PREVIOUS_PANE:   FocusPreviousPane();              break;

    case IDC_OPEN_FILE:             OpenFile();                       break;
    case IDC_CREATE_SHORTCUTS:      OpenCreateShortcutsDialog();      break;
    case IDC_DEV_TOOLS:             ToggleDevToolsWindow(
                                        DEVTOOLS_TOGGLE_ACTION_NONE);
                                    break;
    case IDC_DEV_TOOLS_CONSOLE:     ToggleDevToolsWindow(
                                        DEVTOOLS_TOGGLE_ACTION_SHOW_CONSOLE);
                                    break;
    case IDC_DEV_TOOLS_INSPECT:     ToggleDevToolsWindow(
                                        DEVTOOLS_TOGGLE_ACTION_INSPECT);
                                    break;
    case IDC_TASK_MANAGER:          OpenTaskManager(false);           break;
    case IDC_VIEW_BACKGROUND_PAGES: OpenTaskManager(true);            break;
    case IDC_FEEDBACK:              OpenBugReportDialog();            break;

    case IDC_SHOW_BOOKMARK_BAR:     ToggleBookmarkBar();              break;
    case IDC_PROFILING_ENABLED:     Profiling::Toggle();              break;

    case IDC_SHOW_BOOKMARK_MANAGER: OpenBookmarkManager();            break;
    case IDC_SHOW_APP_MENU:         ShowAppMenu();                    break;
    case IDC_SHOW_HISTORY:          ShowHistoryTab();                 break;
    case IDC_SHOW_DOWNLOADS:        ShowDownloadsTab();               break;
    case IDC_MANAGE_EXTENSIONS:     ShowExtensionsTab();              break;
    case IDC_SYNC_BOOKMARKS:        OpenSyncMyBookmarksDialog();      break;
    case IDC_OPTIONS:               OpenOptionsDialog();              break;
    case IDC_EDIT_SEARCH_ENGINES:   OpenSearchEngineOptionsDialog();  break;
    case IDC_VIEW_PASSWORDS:        OpenPasswordManager();            break;
    case IDC_CLEAR_BROWSING_DATA:   OpenClearBrowsingDataDialog();    break;
    case IDC_IMPORT_SETTINGS:       OpenImportSettingsDialog();       break;
    case IDC_ABOUT:                 OpenAboutChromeDialog();          break;
    case IDC_UPGRADE_DIALOG:        OpenUpdateChromeDialog();         break;
    case IDC_VIEW_INCOMPATIBILITIES: ShowAboutConflictsTab();         break;
    case IDC_HELP_PAGE:             ShowHelpTab();                    break;
#if defined(OS_CHROMEOS)
    case IDC_FILE_MANAGER:          OpenFileManager();                break;
    case IDC_SYSTEM_OPTIONS:        OpenSystemOptionsDialog();        break;
    case IDC_INTERNET_OPTIONS:      OpenInternetOptionsDialog();      break;
    case IDC_LANGUAGE_OPTIONS:      OpenLanguageOptionsDialog();      break;
#endif
    case IDC_SHOW_SYNC_SETUP:       ShowSyncSetup();                  break;
    case IDC_TOGGLE_SPEECH_INPUT:   ToggleSpeechInput();              break;

    default:
      LOG(WARNING) << "Received Unimplemented Command: " << id;
      break;
  }
}
