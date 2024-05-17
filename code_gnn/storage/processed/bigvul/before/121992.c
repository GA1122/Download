void AppListController::PopulateViewFromProfile(Profile* requested_profile) {
#if !defined(USE_AURA)
  if (requested_profile == profile())
    return;
#endif

  SetProfile(requested_profile);

  view_delegate_ = new AppListViewDelegate(CreateControllerDelegate(),
                                           profile());
  current_view_ = new app_list::AppListView(view_delegate_);
  gfx::Point cursor = gfx::Screen::GetNativeScreen()->GetCursorScreenPoint();
  current_view_->InitAsBubble(NULL,
                              &pagination_model_,
                              NULL,
                              cursor,
                              views::BubbleBorder::FLOAT,
                              false  );
  HWND hwnd = GetAppListHWND();

  if (base::win::GetVersion() <= base::win::VERSION_VISTA) {
    LONG_PTR ex_styles = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
    ex_styles |= WS_EX_TOOLWINDOW;
    SetWindowLongPtr(hwnd, GWL_EXSTYLE, ex_styles);
  }

  if (base::win::GetVersion() > base::win::VERSION_VISTA) {
    BOOL disable_value = TRUE;
    ::DwmSetWindowAttribute(hwnd,
                            DWMWA_DISALLOW_PEEK,
                            &disable_value,
                            sizeof(disable_value));
  }

  ui::win::SetAppIdForWindow(GetAppModelId(), hwnd);
  CommandLine relaunch = GetAppListCommandLine();
  string16 app_name(GetAppListShortcutName());
  ui::win::SetRelaunchDetailsForWindow(
      relaunch.GetCommandLineString(), app_name, hwnd);
  ::SetWindowText(hwnd, app_name.c_str());
  string16 icon_path = GetAppListIconPath();
  ui::win::SetAppIconForWindow(icon_path, hwnd);
}
