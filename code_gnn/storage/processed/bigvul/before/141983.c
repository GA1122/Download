void AutofillExternalDelegate::InsertDataListValues(
    std::vector<Suggestion>* suggestions) {
  if (data_list_values_.empty())
    return;

  std::set<base::string16> data_list_set(data_list_values_.begin(),
                                         data_list_values_.end());
  base::EraseIf(*suggestions, [&data_list_set](const Suggestion& suggestion) {
    return suggestion.frontend_id == POPUP_ITEM_ID_AUTOCOMPLETE_ENTRY &&
           base::ContainsKey(data_list_set, suggestion.value);
  });

#if !defined(OS_ANDROID)
  if (!suggestions->empty()) {
    suggestions->insert(suggestions->begin(), Suggestion());
    (*suggestions)[0].frontend_id = POPUP_ITEM_ID_SEPARATOR;
  }
#endif

  suggestions->insert(suggestions->begin(), data_list_values_.size(),
                      Suggestion());
  for (size_t i = 0; i < data_list_values_.size(); i++) {
    (*suggestions)[i].value = data_list_values_[i];
    (*suggestions)[i].label = data_list_labels_[i];
    (*suggestions)[i].frontend_id = POPUP_ITEM_ID_DATALIST_ENTRY;
  }
}
