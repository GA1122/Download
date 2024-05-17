void Browser::OverrideEncoding(int encoding_id) {
  content::RecordAction(UserMetricsAction("OverrideEncoding"));
  const std::string selected_encoding =
      CharacterEncoding::GetCanonicalEncodingNameByCommandId(encoding_id);
  WebContents* contents = chrome::GetActiveWebContents(this);
  if (!selected_encoding.empty() && contents)
     contents->SetOverrideEncoding(selected_encoding);
  std::string new_selected_encoding_list;
  if (CharacterEncoding::UpdateRecentlySelectedEncoding(
        profile_->GetPrefs()->GetString(prefs::kRecentlySelectedEncoding),
        encoding_id,
        &new_selected_encoding_list)) {
    profile_->GetPrefs()->SetString(prefs::kRecentlySelectedEncoding,
                                    new_selected_encoding_list);
  }
}
