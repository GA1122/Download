void PanelBrowserView::OnDisplayChanged() {
  BrowserView::OnDisplayChanged();
  panel_->manager()->display_settings_provider()->OnDisplaySettingsChanged();
}
