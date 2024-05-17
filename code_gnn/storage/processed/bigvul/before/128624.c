ContextualSearchDelegate::ContextualSearchDelegate(
    net::URLRequestContextGetter* url_request_context,
    TemplateURLService* template_url_service,
    const ContextualSearchDelegate::SearchTermResolutionCallback&
        search_term_callback,
    const ContextualSearchDelegate::SurroundingTextCallback&
        surrounding_callback,
    const ContextualSearchDelegate::IcingCallback& icing_callback)
    : url_request_context_(url_request_context),
      template_url_service_(template_url_service),
      search_term_callback_(search_term_callback),
      surrounding_callback_(surrounding_callback),
      icing_callback_(icing_callback) {
  field_trial_.reset(new ContextualSearchFieldTrial());
}
