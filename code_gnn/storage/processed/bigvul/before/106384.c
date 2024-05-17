void OfflineLoadPage::GetNormalOfflineStrings(
    const string16& failed_url, DictionaryValue* strings) const {
  strings->SetString("title", tab()->GetTitle());

  strings->SetString("display_icon", "none");
  strings->SetString("icon", string16());

  strings->SetString(
      "msg",
      l10n_util::GetStringFUTF16(IDS_SITE_OFFLINE_LOAD_DESCRIPTION,
                                 failed_url));
}
