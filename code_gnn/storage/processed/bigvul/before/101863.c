void Browser::UpdateCommandsForTabState() {
  TabContents* current_tab = GetSelectedTabContents();
  TabContentsWrapper* current_tab_wrapper = GetSelectedTabContentsWrapper();
  if (!current_tab || !current_tab_wrapper)   
    return;

  NavigationController& nc = current_tab->controller();
  command_updater_.UpdateCommandEnabled(IDC_BACK, nc.CanGoBack());
  command_updater_.UpdateCommandEnabled(IDC_FORWARD, nc.CanGoForward());
  command_updater_.UpdateCommandEnabled(IDC_RELOAD,
                                        CanReloadContents(current_tab));
  command_updater_.UpdateCommandEnabled(IDC_RELOAD_IGNORING_CACHE,
                                        CanReloadContents(current_tab));

  command_updater_.UpdateCommandEnabled(IDC_DUPLICATE_TAB,
      !is_app() && CanDuplicateContentsAt(active_index()));

  window_->SetStarredState(
      current_tab_wrapper->bookmark_tab_helper()->is_starred());
  command_updater_.UpdateCommandEnabled(IDC_VIEW_SOURCE,
      current_tab->controller().CanViewSource());
  command_updater_.UpdateCommandEnabled(IDC_EMAIL_PAGE_LOCATION,
      current_tab->ShouldDisplayURL() && current_tab->GetURL().is_valid());
  if (is_devtools())
      command_updater_.UpdateCommandEnabled(IDC_OPEN_FILE, false);

  bool is_chrome_internal = HasInternalURL(nc.GetActiveEntry());
  command_updater_.UpdateCommandEnabled(IDC_ENCODING_MENU,
      !is_chrome_internal && SavePackage::IsSavableContents(
          current_tab->contents_mime_type()));

#if !defined(OS_MACOSX)
  command_updater_.UpdateCommandEnabled(IDC_CREATE_SHORTCUTS,
      web_app::IsValidUrl(current_tab->GetURL()));
#endif

  UpdateCommandsForContentRestrictionState();
  UpdateCommandsForBookmarkEditing();
}