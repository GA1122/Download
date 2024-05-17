int ContextualSearchFieldTrial::GetSurroundingSize() {
  return GetIntParamValueOrDefault(kContextualSearchSurroundingSizeParamName,
                                   kContextualSearchDefaultContentSize,
                                   &is_surrounding_size_cached_,
                                   &surrounding_size_);
}
