bool RenderViewImpl::ShouldUpdateSelectionTextFromContextMenuParams(
    const string16& selection_text,
    size_t selection_text_offset,
    const ui::Range& selection_range,
    const ContextMenuParams& params) {
  string16 trimmed_selection_text;
  if (!selection_text.empty() && !selection_range.is_empty()) {
    const int start = selection_range.GetMin() - selection_text_offset;
    const size_t length = selection_range.length();
    if (start >= 0 && start + length <= selection_text.length()) {
      TrimWhitespace(selection_text.substr(start, length), TRIM_ALL,
                     &trimmed_selection_text);
    }
  }
  string16 trimmed_params_text;
  TrimWhitespace(params.selection_text, TRIM_ALL, &trimmed_params_text);
  return trimmed_params_text != trimmed_selection_text;
}
