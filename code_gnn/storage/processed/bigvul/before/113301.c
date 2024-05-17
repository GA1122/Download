void PanelBrowserView::Init() {
  if (!panel_->manager()->display_settings_provider()->is_full_screen()) {
  }

  BrowserView::Init();

  GetWidget()->non_client_view()->SetAccessibleName(
      l10n_util::GetStringUTF16(IDS_PRODUCT_NAME));

  registrar_.Add(
      this,
      chrome::NOTIFICATION_WINDOW_CLOSED,
      content::Source<gfx::NativeWindow>(frame()->GetNativeWindow()));
}
