void BrowserCommandController::UpdateCommandsForFullscreenMode(
    FullScreenMode fullscreen_mode) {
  const bool show_main_ui =
      IsShowingMainUI(fullscreen_mode != FULLSCREEN_DISABLED);
  bool main_not_fullscreen = show_main_ui &&
                             (fullscreen_mode == FULLSCREEN_DISABLED);

  command_updater_.UpdateCommandEnabled(IDC_OPEN_CURRENT_URL, show_main_ui);

  command_updater_.UpdateCommandEnabled(
      IDC_SHOW_AS_TAB,
      !browser_->is_type_tabbed() && fullscreen_mode == FULLSCREEN_DISABLED);

  command_updater_.UpdateCommandEnabled(IDC_FOCUS_TOOLBAR, show_main_ui);
  command_updater_.UpdateCommandEnabled(IDC_FOCUS_LOCATION, show_main_ui);
  command_updater_.UpdateCommandEnabled(IDC_FOCUS_SEARCH, show_main_ui);
  command_updater_.UpdateCommandEnabled(
      IDC_FOCUS_MENU_BAR, main_not_fullscreen);
  command_updater_.UpdateCommandEnabled(
      IDC_FOCUS_NEXT_PANE, main_not_fullscreen);
  command_updater_.UpdateCommandEnabled(
      IDC_FOCUS_PREVIOUS_PANE, main_not_fullscreen);
  command_updater_.UpdateCommandEnabled(
      IDC_FOCUS_BOOKMARKS, main_not_fullscreen);

  command_updater_.UpdateCommandEnabled(IDC_DEVELOPER_MENU, show_main_ui);
  command_updater_.UpdateCommandEnabled(IDC_FEEDBACK, show_main_ui);
  command_updater_.UpdateCommandEnabled(IDC_SHOW_SYNC_SETUP,
      show_main_ui && profile()->GetOriginalProfile()->IsSyncAccessible());

  const bool options_enabled = show_main_ui &&
      IncognitoModePrefs::GetAvailability(
          profile()->GetPrefs()) != IncognitoModePrefs::FORCED;
  command_updater_.UpdateCommandEnabled(IDC_OPTIONS, options_enabled);
  command_updater_.UpdateCommandEnabled(IDC_IMPORT_SETTINGS, options_enabled);

  command_updater_.UpdateCommandEnabled(IDC_EDIT_SEARCH_ENGINES, show_main_ui);
  command_updater_.UpdateCommandEnabled(IDC_VIEW_PASSWORDS, show_main_ui);
  command_updater_.UpdateCommandEnabled(IDC_ABOUT, show_main_ui);
  command_updater_.UpdateCommandEnabled(IDC_SHOW_APP_MENU, show_main_ui);
#if defined (ENABLE_PROFILING) && !defined(NO_TCMALLOC)
  command_updater_.UpdateCommandEnabled(IDC_PROFILING_ENABLED, show_main_ui);
#endif

  bool fullscreen_enabled = !browser_->is_type_panel() &&
                            !browser_->is_app() &&
                            fullscreen_mode != FULLSCREEN_METRO_SNAP;
#if defined(OS_MACOSX)
  int tabIndex = chrome::IndexOfFirstBlockedTab(browser_->tab_strip_model());
  bool has_blocked_tab = tabIndex != browser_->tab_strip_model()->count();
  fullscreen_enabled &= !has_blocked_tab;
#endif

  command_updater_.UpdateCommandEnabled(IDC_FULLSCREEN, fullscreen_enabled);
  command_updater_.UpdateCommandEnabled(IDC_PRESENTATION_MODE,
                                        fullscreen_enabled);

  UpdateCommandsForBookmarkBar();
  UpdateCommandsForMultipleProfiles();
}
