void ContextualSearchDelegate::ExtractMentionsStartEnd(
    const base::ListValue& mentions_list,
    int* startResult,
    int* endResult) {
  int int_value;
  if (mentions_list.GetInteger(0, &int_value))
    *startResult = std::max(0, int_value);
  if (mentions_list.GetInteger(1, &int_value))
    *endResult = std::max(0, int_value);
}
