void HTMLInputElement::ParserDidSetAttributes() {
  DCHECK(parsing_in_progress_);
  InitializeTypeInParsing();
}
