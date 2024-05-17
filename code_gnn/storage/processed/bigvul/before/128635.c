void ContextualSearchDelegate::SetDiscourseContextAndAddToHeader(
    const ContextualSearchContext& context) {
  search_term_fetcher_->AddExtraRequestHeader(GetDiscourseContext(context));
}
