string16 AutocompleteEditModel::GetDesiredTLD() const {
  return (control_key_state_ == DOWN_WITHOUT_CHANGE &&
          inline_autocomplete_text_.empty() && !KeywordIsSelected())?
    ASCIIToUTF16("com") : string16();
}
