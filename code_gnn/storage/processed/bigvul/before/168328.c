void BrowserView::OnThemeChanged() {
  if (!IsRegularOrGuestSession()) {
    base::AutoReset<bool> reset(&handling_theme_changed_, true);
#if defined(USE_AURA)
    ui::NativeThemeDarkAura::instance()->NotifyObservers();
#endif
    ui::NativeTheme::GetInstanceForNativeUi()->NotifyObservers();
  }

  views::View::OnThemeChanged();
}
