bool ZeroSuggestProvider::StoreSuggestionResponse(
    const std::string& json_data,
    const base::Value& parsed_data) {
  if (!OmniboxFieldTrial::InZeroSuggestPersonalizedFieldTrial() ||
      json_data.empty())
    return false;
  client()->GetPrefs()->SetString(omnibox::kZeroSuggestCachedResults,
                                  json_data);

  const base::ListValue* root_list = NULL;
  const base::ListValue* results_list = NULL;
  if (parsed_data.GetAsList(&root_list) &&
      root_list->GetList(1, &results_list) &&
      results_list->empty())
    return false;

  if (results_from_cache_)
    done_ = true;

  return results_from_cache_;
}
