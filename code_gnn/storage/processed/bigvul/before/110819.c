bool AutocompleteEditModel::UseVerbatimInstant() {
#if defined(OS_MACOSX)
  const AutocompleteInput& input = autocomplete_controller_->input();
  if (input.prevent_inline_autocomplete())
    return true;
#endif

  if (view_->DeleteAtEndPressed() || (popup_->selected_line() != 0) ||
      just_deleted_text_)
    return true;

  size_t start, end;
  view_->GetSelectionBounds(&start, &end);
  return (start != end) || (start != view_->GetText().length());
}
