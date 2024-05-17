void GDataEntry::SetBaseNameFromTitle() {
   base_name_ = EscapeUtf8FileName(title_);
 }
