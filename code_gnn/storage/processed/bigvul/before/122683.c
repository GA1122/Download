bool Extension::LoadIcons(string16* error) {
  if (!manifest_->HasKey(keys::kIcons))
    return true;
  DictionaryValue* icons_value = NULL;
  if (!manifest_->GetDictionary(keys::kIcons, &icons_value)) {
    *error = ASCIIToUTF16(errors::kInvalidIcons);
    return false;
  }

  return manifest_handler_helpers::LoadIconsFromDictionary(
      icons_value,
      extension_misc::kExtensionIconSizes,
      extension_misc::kNumExtensionIconSizes,
      &icons_,
      error);
}
