  void CreateSearchContextAndRequestSearchTerm(
      const std::string& selected_text,
      const base::string16& surrounding_text,
      int start_offset,
      int end_offset) {
    test_context_ = new ContextualSearchContext(
        selected_text, true, GURL(kSomeSpecificBasePage), "utf-8");
    delegate_->set_context_for_testing(test_context_);

    test_context_->start_offset = start_offset;
    test_context_->end_offset = end_offset;
    test_context_->surrounding_text = surrounding_text;
    delegate_->ContinueSearchTermResolutionRequest();
    fetcher_ = test_factory_.GetFetcherByID(
        ContextualSearchDelegate::kContextualSearchURLFetcherID);
    ASSERT_TRUE(fetcher_);
    ASSERT_TRUE(fetcher());
  }
