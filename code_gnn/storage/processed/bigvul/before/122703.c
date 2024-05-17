bool Extension::LoadThemeFeatures(string16* error) {
  if (!manifest_->HasKey(keys::kTheme))
    return true;
  DictionaryValue* theme_value = NULL;
  if (!manifest_->GetDictionary(keys::kTheme, &theme_value)) {
    *error = ASCIIToUTF16(errors::kInvalidTheme);
    return false;
  }
  if (!LoadThemeImages(theme_value, error))
    return false;
  if (!LoadThemeColors(theme_value, error))
    return false;
  if (!LoadThemeTints(theme_value, error))
    return false;
  if (!LoadThemeDisplayProperties(theme_value, error))
    return false;

  return true;
}
