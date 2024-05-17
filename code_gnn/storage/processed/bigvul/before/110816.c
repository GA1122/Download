AutocompleteEditModel::State::State(bool user_input_in_progress,
                                    const string16& user_text,
                                    const string16& keyword,
                                    bool is_keyword_hint)
    : user_input_in_progress(user_input_in_progress),
      user_text(user_text),
      keyword(keyword),
      is_keyword_hint(is_keyword_hint) {
}
