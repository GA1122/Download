void AutocompleteEditModel::GetDataForURLExport(GURL* url,
                                                string16* title,
                                                SkBitmap* favicon) {
  AutocompleteMatch match;
  GetInfoForCurrentText(&match, NULL);
  *url = match.destination_url;
  if (*url == URLFixerUpper::FixupURL(UTF16ToUTF8(permanent_text_),
                                      std::string())) {
    *title = controller_->GetTitle();
    *favicon = controller_->GetFavicon();
  }
}
