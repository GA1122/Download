void StatusBubbleGtk::SetStatus(const string16& status_text_wide) {
  std::string status_text = UTF16ToUTF8(status_text_wide);
  if (status_text_ == status_text)
    return;

  status_text_ = status_text;
  if (!status_text_.empty())
    SetStatusTextTo(status_text_);
  else if (!url_text_.empty())
    SetStatusTextTo(url_text_);
  else
    SetStatusTextTo(std::string());
}
