 std::string OfflineLoadPage::GetHTMLContents() {
   DictionaryValue strings;
   int64 time_to_wait = std::max(
      static_cast<int64>(0),
      kMaxBlankPeriod -
      NetworkStateNotifier::GetOfflineDuration().InMilliseconds());
  strings.SetInteger("time_to_wait", static_cast<int>(time_to_wait));
  SetString(&strings, "heading", IDS_OFFLINE_LOAD_HEADLINE);
  SetString(&strings, "try_loading", IDS_OFFLINE_TRY_LOADING);
  SetString(&strings, "network_settings", IDS_OFFLINE_NETWORK_SETTINGS);

  strings.SetBoolean("show_activation", ShowActivationMessage());

  bool rtl = base::i18n::IsRTL();
  strings.SetString("textdirection", rtl ? "rtl" : "ltr");

  string16 failed_url(ASCIIToUTF16(url().spec()));
  if (rtl)
    base::i18n::WrapStringWithLTRFormatting(&failed_url);
  strings.SetString("url", failed_url);

  Profile* profile = tab()->profile();
  DCHECK(profile);
  const Extension* extension = NULL;
  ExtensionService* extensions_service = profile->GetExtensionService();
  if (extensions_service)
    extension = extensions_service->GetExtensionByWebExtent(url());

  if (extension)
    GetAppOfflineStrings(extension, failed_url, &strings);
  else
    GetNormalOfflineStrings(failed_url, &strings);

  base::StringPiece html(
      ResourceBundle::GetSharedInstance().GetRawDataResource(
          IDR_OFFLINE_LOAD_HTML));
  return jstemplate_builder::GetI18nTemplateHtml(html, &strings);
}
