void RenderFrameImpl::OnSetCompositionFromExistingText(
    int start,
    int end,
    const std::vector<blink::WebImeTextSpan>& ime_text_spans) {
  ImeEventGuard guard(GetRenderWidget());
  frame_->SetCompositionFromExistingText(start, end, ime_text_spans);
}
