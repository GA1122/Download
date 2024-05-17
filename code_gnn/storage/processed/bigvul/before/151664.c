Browser::CreateParams::CreateParams(Profile* profile)
    : type(TYPE_TABBED),
      profile(profile),
      trusted_source(false),
      initial_show_state(ui::SHOW_STATE_DEFAULT),
      is_session_restore(false),
      window(NULL) {}
