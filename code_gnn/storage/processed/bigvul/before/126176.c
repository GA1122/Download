Browser::CreateParams::CreateParams(Type type, Profile* profile)
    : type(type),
      profile(profile),
      host_desktop_type(kDefaultHostDesktopType),
      app_type(APP_TYPE_HOST),
      initial_show_state(ui::SHOW_STATE_DEFAULT),
      is_session_restore(false),
      window(NULL) {
}
