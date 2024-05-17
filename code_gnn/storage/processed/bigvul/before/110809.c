void AutocompleteEditModel::RestoreState(const State& state) {
  if (state.user_input_in_progress) {
    keyword_ = state.keyword;
    is_keyword_hint_ = state.is_keyword_hint;
    view_->SetUserText(state.user_text,
        DisplayTextFromUserText(state.user_text), false);
  }
}
