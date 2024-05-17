  void CheckSuggestions(int expected_page_id,
                        const Suggestion& suggestion0,
                        const Suggestion& suggestion1,
                        const Suggestion& suggestion2) {
    std::vector<Suggestion> suggestion_vector;
    suggestion_vector.push_back(suggestion0);
    suggestion_vector.push_back(suggestion1);
    suggestion_vector.push_back(suggestion2);
    external_delegate_->CheckSuggestions(expected_page_id, 3,
                                         &suggestion_vector[0]);
  }
