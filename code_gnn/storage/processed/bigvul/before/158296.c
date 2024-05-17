void RenderWidgetHostImpl::ImeSetComposition(
    const base::string16& text,
    const std::vector<ui::ImeTextSpan>& ime_text_spans,
    const gfx::Range& replacement_range,
    int selection_start,
    int selection_end) {
  GetWidgetInputHandler()->ImeSetComposition(
      text, ime_text_spans, replacement_range, selection_start, selection_end);
}
