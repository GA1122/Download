void StatusBubbleGtk::UserChangedTheme() {
  if (theme_provider_->UseGtkTheme()) {
    gtk_widget_modify_fg(label_, GTK_STATE_NORMAL, NULL);
    gtk_widget_modify_bg(container_.get(), GTK_STATE_NORMAL, NULL);
  } else {
    GdkColor bookmark_text =
        theme_provider_->GetGdkColor(BrowserThemeProvider::COLOR_BOOKMARK_TEXT);
    gtk_widget_modify_fg(label_, GTK_STATE_NORMAL, &bookmark_text);

    GdkColor toolbar_color =
        theme_provider_->GetGdkColor(BrowserThemeProvider::COLOR_TOOLBAR);
    gtk_widget_modify_bg(container_.get(), GTK_STATE_NORMAL, &toolbar_color);
  }

  gtk_util::SetRoundedWindowBorderColor(container_.get(),
                                        theme_provider_->GetBorderColor());
}
