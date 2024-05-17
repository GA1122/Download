SkColor BrowserWindowGtk::GetInfoBarSeparatorColor() const {
  GtkThemeService* theme_service = GtkThemeService::GetFrom(
      browser()->profile());
  return gfx::GdkColorToSkColor(theme_service->GetBorderColor());
}
