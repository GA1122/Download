void RenderWidgetHostImpl::ImeSetComposition(
    const base::string16& text,
    const std::vector<blink::WebCompositionUnderline>& underlines,
    const gfx::Range& replacement_range,
    int selection_start,
    int selection_end) {
  Send(new InputMsg_ImeSetComposition(
            GetRoutingID(), text, underlines, replacement_range,
            selection_start, selection_end));
}
