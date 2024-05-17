int ContextualSearchFieldTrial::GetIcingSurroundingSize() {
  return GetIntParamValueOrDefault(
      kContextualSearchIcingSurroundingSizeParamName,
      kContextualSearchDefaultIcingSurroundingSize,
      &is_icing_surrounding_size_cached_, &icing_surrounding_size_);
}
