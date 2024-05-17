Browser::CreateParams::CreateParams(Type type, Profile* profile)
    : type(type),
      profile(profile),
      trusted_source(false),
      initial_show_state(ui::SHOW_STATE_DEFAULT),
      is_session_restore(false),
      window(NULL) {}
