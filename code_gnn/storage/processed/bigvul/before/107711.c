StatusBubbleGtk::StatusBubbleGtk(Profile* profile)
    : theme_provider_(GtkThemeProvider::GetFrom(profile)),
      padding_(NULL),
      label_(NULL),
      flip_horizontally_(false),
      y_offset_(0),
      download_shelf_is_visible_(false),
      last_mouse_location_(0, 0),
      last_mouse_left_content_(false),
      ignore_next_left_content_(false) {
  InitWidgets();

  theme_provider_->InitThemesFor(this);
  registrar_.Add(this, NotificationType::BROWSER_THEME_CHANGED,
                 NotificationService::AllSources());
}
