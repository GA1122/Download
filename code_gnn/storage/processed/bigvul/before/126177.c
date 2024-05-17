Browser::CreateParams::CreateParams(Type type,
                                    Profile* profile,
                                    chrome::HostDesktopType host_desktop_type)
    : type(type),
      profile(profile),
      host_desktop_type(host_desktop_type),
      app_type(APP_TYPE_HOST),
      initial_show_state(ui::SHOW_STATE_DEFAULT),
      is_session_restore(false),
      window(NULL) {
}
