void BrowserWindowGtk::SetBackgroundColor() {
  Profile* profile = browser()->profile();
  GtkThemeService* theme_provider = GtkThemeService::GetFrom(profile);
  int frame_color_id;
  if (UsingCustomPopupFrame()) {
    frame_color_id = ThemeService::COLOR_TOOLBAR;
  } else if (DrawFrameAsActive()) {
    frame_color_id = browser()->profile()->IsOffTheRecord()
       ? ThemeService::COLOR_FRAME_INCOGNITO
       : ThemeService::COLOR_FRAME;
  } else {
    frame_color_id = browser()->profile()->IsOffTheRecord()
       ? ThemeService::COLOR_FRAME_INCOGNITO_INACTIVE
       : ThemeService::COLOR_FRAME_INACTIVE;
  }

  SkColor frame_color = theme_provider->GetColor(frame_color_id);

  GdkColor frame_color_gdk = gfx::SkColorToGdkColor(frame_color);
  gtk_widget_modify_bg(GTK_WIDGET(window_), GTK_STATE_NORMAL,
                       &frame_color_gdk);

  gtk_widget_modify_bg(contents_vsplit_, GTK_STATE_NORMAL, &frame_color_gdk);
  gtk_widget_modify_bg(contents_hsplit_, GTK_STATE_NORMAL, &frame_color_gdk);

  color_utils::HSL hsl = { -1, 0.5, 0.65 };
  SkColor frame_prelight_color = color_utils::HSLShift(frame_color, hsl);
  GdkColor frame_prelight_color_gdk =
      gfx::SkColorToGdkColor(frame_prelight_color);
  gtk_widget_modify_bg(contents_hsplit_, GTK_STATE_PRELIGHT,
      &frame_prelight_color_gdk);
  gtk_widget_modify_bg(contents_vsplit_, GTK_STATE_PRELIGHT,
      &frame_prelight_color_gdk);

  GdkColor border_color = theme_provider->GetBorderColor();
  gtk_widget_modify_bg(toolbar_border_, GTK_STATE_NORMAL, &border_color);
}
