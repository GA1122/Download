void RenderView::OnImeSetComposition(
    const string16& text,
    const std::vector<WebKit::WebCompositionUnderline>& underlines,
    int selection_start,
    int selection_end) {
  if (!pepper_delegate_.IsPluginFocused()) {
    RenderWidget::OnImeSetComposition(text,
                                      underlines,
                                      selection_start,
                                      selection_end);
  }
}
