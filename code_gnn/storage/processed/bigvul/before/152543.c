void RenderFrameImpl::SimulateImeSetComposition(
    const base::string16& text,
    const std::vector<blink::WebImeTextSpan>& ime_text_spans,
    int selection_start,
    int selection_end) {
  GetMainFrameRenderWidget()->OnImeSetComposition(
      text, ime_text_spans, gfx::Range::InvalidRange(), selection_start,
      selection_end);
}
