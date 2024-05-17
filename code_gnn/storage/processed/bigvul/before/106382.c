void OfflineLoadPage::GetAppOfflineStrings(
    const Extension* app,
    const string16& failed_url,
    DictionaryValue* strings) const {
  strings->SetString("title", app->name());

  GURL icon_url = app->GetIconURL(Extension::EXTENSION_ICON_LARGE,
                                  ExtensionIconSet::MATCH_EXACTLY);
  if (icon_url.is_empty()) {
    strings->SetString("display_icon", "none");
    strings->SetString("icon", string16());
  } else {
    strings->SetString("display_icon", "block");
    strings->SetString("icon", icon_url.spec());
  }

  strings->SetString(
      "msg",
      l10n_util::GetStringFUTF16(IDS_APP_OFFLINE_LOAD_DESCRIPTION,
                                 failed_url));
}
