string16 LearnMoreInfoBar::GetMessageTextWithOffset(size_t* link_offset) const {
  string16 text = message_;
  text.push_back(' ');   
  *link_offset = text.size();
  return text;
}
