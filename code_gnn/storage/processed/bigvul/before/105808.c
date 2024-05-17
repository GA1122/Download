void Label::SetText(const std::wstring& text) {
  text_ = WideToUTF16Hack(text);
  url_set_ = false;
  text_size_valid_ = false;
  PreferredSizeChanged();
  SchedulePaint();
}
