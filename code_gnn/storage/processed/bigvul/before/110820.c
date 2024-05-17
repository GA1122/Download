string16 AutocompleteEditModel::UserTextFromDisplayText(
    const string16& text) const {
  return KeywordIsSelected() ? (keyword_ + char16(' ') + text) : text;
}
