void Document::PlatformColorsChanged() {
  if (!IsActive())
    return;

  GetStyleEngine().PlatformColorsChanged();
}
