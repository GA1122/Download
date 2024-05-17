  void SetSurroundingContext(const base::string16& surrounding_text,
                             int start_offset,
                             int end_offset) {
    test_context_ = new ContextualSearchContext(
        "Bogus", true, GURL(kSomeSpecificBasePage), "utf-8");
    test_context_->surrounding_text = surrounding_text;
    test_context_->start_offset = start_offset;
    test_context_->end_offset = end_offset;
    delegate_->set_context_for_testing(test_context_);
  }
