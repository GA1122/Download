bool AutocompleteEditModel::KeywordIsSelected() const {
  return !is_keyword_hint_ && !keyword_.empty();
}