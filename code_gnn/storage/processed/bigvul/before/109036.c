void RenderViewImpl::SimulateImeSetComposition(
    const string16& text,
    const std::vector<WebKit::WebCompositionUnderline>& underlines,
    int selection_start,
    int selection_end) {
  OnImeSetComposition(text, underlines, selection_start, selection_end);
}
