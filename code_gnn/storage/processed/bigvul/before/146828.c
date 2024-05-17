void Document::SetContent(const String& content) {
  open();
  parser_->Append(content);
  close();
}
