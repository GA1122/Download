void AutocompleteInput::RemoveForcedQueryStringIfNecessary(Type type,
                                                           string16* text) {
  if (type == FORCED_QUERY && !text->empty() && (*text)[0] == L'?')
    text->erase(0, 1);
}
