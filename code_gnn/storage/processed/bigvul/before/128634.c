void ContextualSearchDelegate::SendSurroundingText(int max_surrounding_chars) {
  const base::string16& surrounding = context_->surrounding_text;

  int surrounding_length = surrounding.length();   
  int num_after_characters = std::min(
      surrounding_length - context_->end_offset, max_surrounding_chars);
  base::string16 after_text = surrounding.substr(
      context_->end_offset, num_after_characters);

  base::TrimWhitespace(after_text, base::TRIM_ALL, &after_text);
  surrounding_callback_.Run(UTF16ToUTF8(after_text));
}
