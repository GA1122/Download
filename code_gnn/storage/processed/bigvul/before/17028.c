  ConstructProps()
      : incognito(false),
        context(nullptr),
        restore_type(oxide::qt::RESTORE_LAST_SESSION_EXITED_CLEANLY),
        location_bar_height(0),
        location_bar_mode(oxide::qt::LOCATION_BAR_MODE_AUTO),
        location_bar_animated(true),
        load_html(false),
        fullscreen(false) {}
