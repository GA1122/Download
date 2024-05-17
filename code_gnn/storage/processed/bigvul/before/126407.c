void BrowserWindowGtk::OnPreferenceChanged(PrefServiceBase* service,
                                           const std::string& pref_name) {
  if (pref_name == prefs::kUseCustomChromeFrame) {
    UpdateCustomFrame();
    ui::SetHideTitlebarWhenMaximizedProperty(
        ui::GetX11WindowFromGtkWidget(GTK_WIDGET(window_)),
        UseCustomFrame() ?
            ui::HIDE_TITLEBAR_WHEN_MAXIMIZED :
            ui::SHOW_TITLEBAR_WHEN_MAXIMIZED);
  } else {
    NOTREACHED() << "Got pref change notification we didn't register for!";
   }
 }
