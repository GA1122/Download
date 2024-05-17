int TabStrip::GetBackgroundResourceId(
    bool* has_custom_image,
    BrowserNonClientFrameView::ActiveState active_state) const {
  if (!TitlebarBackgroundIsTransparent()) {
    return controller_->GetTabBackgroundResourceId(active_state,
                                                   has_custom_image);
  }

  constexpr int kBackgroundIdGlass = IDR_THEME_TAB_BACKGROUND_V;
  *has_custom_image = GetThemeProvider()->HasCustomImage(kBackgroundIdGlass);
  return kBackgroundIdGlass;
}
