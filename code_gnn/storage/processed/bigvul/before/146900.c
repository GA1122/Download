String Document::lastModified() const {
  DateComponents date;
  bool found_date = false;
  if (frame_) {
    if (DocumentLoader* document_loader = Loader()) {
      const AtomicString& http_last_modified =
          document_loader->GetResponse().HttpHeaderField(
              HTTPNames::Last_Modified);
      if (!http_last_modified.IsEmpty()) {
        double date_value = ParseDate(http_last_modified);
        if (!std::isnan(date_value)) {
          date.SetMillisecondsSinceEpochForDateTime(
              ConvertToLocalTime(date_value));
          found_date = true;
        }
      }
    }
  }
  if (!found_date)
    date.SetMillisecondsSinceEpochForDateTime(
        ConvertToLocalTime(CurrentTimeMS()));
  return String::Format("%02d/%02d/%04d %02d:%02d:%02d", date.Month() + 1,
                        date.MonthDay(), date.FullYear(), date.Hour(),
                        date.Minute(), date.Second());
}