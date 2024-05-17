  void recordSearchTermResolutionResponse(
      const ResolvedSearchTerm& resolved_search_term) {
    is_invalid_ = resolved_search_term.is_invalid;
    response_code_ = resolved_search_term.response_code;
    search_term_ = resolved_search_term.search_term;
    display_text_ = resolved_search_term.display_text;
    alternate_term_ = resolved_search_term.alternate_term;
    mid_ = resolved_search_term.mid;
    thumbnail_url_ = resolved_search_term.thumbnail_url;
    caption_ = resolved_search_term.caption;
    prevent_preload_ = resolved_search_term.prevent_preload;
    start_adjust_ = resolved_search_term.selection_start_adjust;
    end_adjust_ = resolved_search_term.selection_end_adjust;
    context_language_ = resolved_search_term.context_language;
  }
