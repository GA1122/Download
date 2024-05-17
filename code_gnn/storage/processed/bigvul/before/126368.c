void BrowserWindowGtk::Init() {
  ui::ActiveWindowWatcherX::AddObserver(this);

  use_custom_frame_pref_.Init(prefs::kUseCustomChromeFrame,
      browser_->profile()->GetPrefs(), this);

  if (!browser_->profile()->IsOffTheRecord()) {
    registrar_.Add(this, chrome::NOTIFICATION_PROFILE_CACHED_INFO_CHANGED,
                   content::NotificationService::AllSources());
  }

  if (ui::GuessWindowManager() == ui::WM_COMPIZ)
    suppress_window_raise_ = true;

  window_ = GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL));
  g_object_set_qdata(G_OBJECT(window_), GetBrowserWindowQuarkKey(), this);
  gtk_widget_add_events(GTK_WIDGET(window_), GDK_BUTTON_PRESS_MASK |
                                             GDK_POINTER_MOTION_MASK);

  gtk_window_util::DisableResizeGrip(window_);

  gtk_window_group_add_window(gtk_window_group_new(), window_);
  g_object_unref(gtk_window_get_group(window_));

  const CommandLine& command_line = *CommandLine::ForCurrentProcess();
  if (browser_->is_app()) {
    std::string app_name = browser_->app_name();
    if (app_name != DevToolsWindow::kDevToolsApp) {
      gtk_window_util::SetWindowCustomClass(window_,
          web_app::GetWMClassFromAppName(app_name));
    }
  } else if (command_line.HasSwitch(switches::kUserDataDir)) {
    const std::string user_data_dir =
        command_line.GetSwitchValueNative(switches::kUserDataDir);
    gtk_window_util::SetWindowCustomClass(window_,
        std::string(gdk_get_program_class()) + " (" + user_data_dir + ")");
  }

  if (browser_->is_type_popup() || browser_->is_type_panel()) {
    gtk_window_set_role(window_, "pop-up");
    InitWidgets();
    SetGeometryHints();
  } else {
    gtk_window_set_role(window_, "browser");
    SetGeometryHints();
    InitWidgets();
  }

  ConnectAccelerators();

  SetBackgroundColor();
  HideUnsupportedWindowFeatures();

  if (UseCustomFrame()) {
    ui::SetHideTitlebarWhenMaximizedProperty(
        ui::GetX11WindowFromGtkWidget(GTK_WIDGET(window_)),
        ui::HIDE_TITLEBAR_WHEN_MAXIMIZED);
  }
}
