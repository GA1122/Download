const OriginAccessEntry& Document::AccessEntryFromURL() {
  if (!access_entry_from_url_) {
    access_entry_from_url_ = WTF::WrapUnique(
        new OriginAccessEntry(Url().Protocol(), Url().Host(),
                              OriginAccessEntry::kAllowRegisterableDomains));
  }
  return *access_entry_from_url_;
}
