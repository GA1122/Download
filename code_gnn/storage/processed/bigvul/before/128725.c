TemplateURLRef::SearchTermsArgs::SearchTermsArgs(
    const base::string16& search_terms)
    : search_terms(search_terms),
      input_type(metrics::OmniboxInputType::INVALID),
      accepted_suggestion(NO_SUGGESTIONS_AVAILABLE),
      cursor_position(base::string16::npos),
      page_classification(metrics::OmniboxEventProto::INVALID_SPEC),
      append_extra_query_params(false),
      force_instant_results(false),
      from_app_list(false),
      contextual_search_params(ContextualSearchParams()) {}
