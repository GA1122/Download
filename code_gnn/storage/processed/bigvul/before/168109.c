  void CheckSuggestions(int expected_page_id, const Suggestion& suggestion0) {
    std::vector<Suggestion> suggestion_vector;
    suggestion_vector.push_back(suggestion0);
    external_delegate_->CheckSuggestions(expected_page_id, 1,
                                         &suggestion_vector[0]);
  }
