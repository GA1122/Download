void AutocompletePopupViewGtk::Observe(NotificationType type,
                                       const NotificationSource& source,
                                       const NotificationDetails& details) {
  DCHECK(type == NotificationType::BROWSER_THEME_CHANGED);

  if (theme_provider_->UseGtkTheme()) {
    border_color_ = theme_provider_->GetBorderColor();

    GtkWidget* fake_tree = gtk_entry_new();
    GtkStyle* style = gtk_rc_get_style(fake_tree);

    background_color_ = style->base[GTK_STATE_NORMAL];
    selected_background_color_ = style->base[GTK_STATE_SELECTED];
    hovered_background_color_ = gtk_util::AverageColors(
        background_color_, selected_background_color_);

    content_text_color_ = style->text[GTK_STATE_NORMAL];
    selected_content_text_color_ = style->text[GTK_STATE_SELECTED];
    url_text_color_ =
        NormalURLColor(style->text[GTK_STATE_NORMAL]);
    url_selected_text_color_ =
        SelectedURLColor(style->text[GTK_STATE_SELECTED],
                         style->base[GTK_STATE_SELECTED]);

    description_text_color_ = style->text[GTK_STATE_NORMAL];
    description_selected_text_color_ = style->text[GTK_STATE_SELECTED];

    g_object_ref_sink(fake_tree);
    g_object_unref(fake_tree);
  } else {
    border_color_ = kBorderColor;
    background_color_ = kBackgroundColor;
    selected_background_color_ = kSelectedBackgroundColor;
    hovered_background_color_ = kHoveredBackgroundColor;

    content_text_color_ = kContentTextColor;
    selected_content_text_color_ = kContentTextColor;
    url_text_color_ = kURLTextColor;
    url_selected_text_color_ = kURLTextColor;
    description_text_color_ = kDescriptionTextColor;
    description_selected_text_color_ = kDescriptionSelectedTextColor;
  }

  gtk_widget_modify_bg(window_, GTK_STATE_NORMAL, &background_color_);
}
