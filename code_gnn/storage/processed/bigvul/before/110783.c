string16 AutocompleteEditModel::DisplayTextFromUserText(
    const string16& text) const {
  return KeywordIsSelected() ?
      KeywordProvider::SplitReplacementStringFromInput(text, false) : text;
}
