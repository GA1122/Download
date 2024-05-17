ZeroSuggestProvider::ZeroSuggestProvider(
    AutocompleteProviderClient* client,
    HistoryURLProvider* history_url_provider,
    AutocompleteProviderListener* listener)
    : BaseSearchProvider(AutocompleteProvider::TYPE_ZERO_SUGGEST, client),
      history_url_provider_(history_url_provider),
      listener_(listener),
      results_from_cache_(false),
      waiting_for_most_visited_urls_request_(false),
      weak_ptr_factory_(this) {
  const TemplateURLService* template_url_service =
      client->GetTemplateURLService();
  if (template_url_service != nullptr) {
    GURL suggest_url(GetContextualSuggestionsUrl());
    UMA_HISTOGRAM_BOOLEAN(
        "Omnibox.ZeroSuggest.Eligible.OnProfileOpen",
        suggest_url.is_valid() &&
            CanSendURL(GURL(kArbitraryInsecureUrlString), suggest_url,
                       template_url_service->GetDefaultSearchProvider(),
                       metrics::OmniboxEventProto::OTHER,
                       template_url_service->search_terms_data(), client));
  }
}
