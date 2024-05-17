void RenderFrameImpl::SimulateImeCommitText(
    const base::string16& text,
    const std::vector<blink::WebImeTextSpan>& ime_text_spans,
    const gfx::Range& replacement_range) {
  GetMainFrameRenderWidget()->OnImeCommitText(text, ime_text_spans,
                                              replacement_range, 0);
}
