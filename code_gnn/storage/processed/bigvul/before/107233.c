DictionaryValue* AutomationProvider::GetDictionaryFromDownloadItem(
    const DownloadItem* download) {
  std::map<DownloadItem::DownloadState, std::string> state_to_string;
  state_to_string[DownloadItem::IN_PROGRESS] = std::string("IN_PROGRESS");
  state_to_string[DownloadItem::CANCELLED] = std::string("CANCELLED");
  state_to_string[DownloadItem::REMOVING] = std::string("REMOVING");
  state_to_string[DownloadItem::COMPLETE] = std::string("COMPLETE");

  std::map<DownloadItem::SafetyState, std::string> safety_state_to_string;
  safety_state_to_string[DownloadItem::SAFE] = std::string("SAFE");
  safety_state_to_string[DownloadItem::DANGEROUS] = std::string("DANGEROUS");
  safety_state_to_string[DownloadItem::DANGEROUS_BUT_VALIDATED] =
      std::string("DANGEROUS_BUT_VALIDATED");

  DictionaryValue* dl_item_value = new DictionaryValue;
  dl_item_value->SetInteger("id", static_cast<int>(download->id()));
  dl_item_value->SetString("url", download->url().spec());
  dl_item_value->SetString("referrer_url", download->referrer_url().spec());
  dl_item_value->SetString("file_name",
                           download->GetFileNameToReportUser().value());
  dl_item_value->SetString("full_path",
                           download->GetTargetFilePath().value());
  dl_item_value->SetBoolean("is_paused", download->is_paused());
  dl_item_value->SetBoolean("open_when_complete",
                            download->open_when_complete());
  dl_item_value->SetBoolean("is_extension_install",
                            download->is_extension_install());
  dl_item_value->SetBoolean("is_temporary", download->is_temporary());
  dl_item_value->SetBoolean("is_otr", download->is_otr());   
  dl_item_value->SetString("state", state_to_string[download->state()]);
  dl_item_value->SetString("safety_state",
                           safety_state_to_string[download->safety_state()]);
  dl_item_value->SetInteger("PercentComplete", download->PercentComplete());

  return dl_item_value;
}
