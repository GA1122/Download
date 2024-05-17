void OmniboxPopupViewGtk::Observe(int type,
                                  const content::NotificationSource& source,
                                  const content::NotificationDetails& details) {
  DCHECK(type == chrome::NOTIFICATION_BROWSER_THEME_CHANGED);

  if (theme_service_->UsingNativeTheme()) {
    gtk_util::UndoForceFontSize(window_);

    border_color_ = theme_service_->GetBorderColor();

    gtk_util::GetTextColors(
        &background_color_, &selected_background_color_,
        &content_text_color_, &selected_content_text_color_);

    hovered_background_color_ = gtk_util::AverageColors(
        background_color_, selected_background_color_);
    url_text_color_ = NormalURLColor(content_text_color_);
    url_selected_text_color_ = SelectedURLColor(selected_content_text_color_,
                                                selected_background_color_);
  } else {
    gtk_util::ForceFontSizePixels(window_, font_.GetFontSize());

    border_color_ = kBorderColor;
    background_color_ = kBackgroundColor;
    selected_background_color_ = kSelectedBackgroundColor;
    hovered_background_color_ = kHoveredBackgroundColor;

    content_text_color_ = kContentTextColor;
    selected_content_text_color_ = kContentTextColor;
    url_text_color_ = kURLTextColor;
    url_selected_text_color_ = kURLTextColor;
  }

  content_dim_text_color_ =
      gtk_util::AverageColors(content_text_color_,
                              background_color_);
  selected_content_dim_text_color_ =
      gtk_util::AverageColors(selected_content_text_color_,
                              selected_background_color_);

  gtk_widget_modify_bg(window_, GTK_STATE_NORMAL, &background_color_);
}
