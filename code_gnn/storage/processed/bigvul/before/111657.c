void GDataEntry::SetFileNameFromTitle() {
  file_name_ = EscapeUtf8FileName(title_);
}
