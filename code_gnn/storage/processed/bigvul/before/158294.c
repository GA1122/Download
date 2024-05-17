void RenderWidgetHostImpl::ImeCommitText(
    const base::string16& text,
    const std::vector<ui::ImeTextSpan>& ime_text_spans,
    const gfx::Range& replacement_range,
    int relative_cursor_pos) {
  GetWidgetInputHandler()->ImeCommitText(text, ime_text_spans,
                                         replacement_range, relative_cursor_pos,
                                         base::OnceClosure());
}
