ContextualSearchDelegate::GetResolvedSearchTermFromJson(
    int response_code,
    const std::string& json_string) {
  std::string search_term;
  std::string display_text;
  std::string alternate_term;
  std::string mid;
  std::string prevent_preload;
  int mention_start = 0;
  int mention_end = 0;
  int start_adjust = 0;
  int end_adjust = 0;
  std::string context_language;
  std::string thumbnail_url = "";
  std::string caption = "";

  DecodeSearchTermFromJsonResponse(
      json_string, &search_term, &display_text, &alternate_term, &mid,
      &prevent_preload, &mention_start, &mention_end, &context_language,
      &thumbnail_url, &caption);
  if (mention_start != 0 || mention_end != 0) {
    if (mention_start >= mention_end ||
        (mention_end - mention_start) > kContextualSearchMaxSelection ||
        mention_end <= context_->start_offset ||
        mention_start >= context_->end_offset) {
      start_adjust = 0;
      end_adjust = 0;
    } else {
      start_adjust = mention_start - context_->start_offset;
      end_adjust = mention_end - context_->end_offset;
    }
  }
  bool is_invalid = response_code == net::URLFetcher::RESPONSE_CODE_INVALID;
  return std::unique_ptr<ResolvedSearchTerm>(new ResolvedSearchTerm(
      is_invalid, response_code, search_term, display_text, alternate_term, mid,
      prevent_preload == kDoPreventPreloadValue, start_adjust, end_adjust,
      context_language, thumbnail_url, caption));
}
