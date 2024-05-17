void RenderWidgetHostImpl::ImeSetComposition(
    const string16& text,
    const std::vector<WebKit::WebCompositionUnderline>& underlines,
    int selection_start,
    int selection_end) {
  Send(new ViewMsg_ImeSetComposition(
            GetRoutingID(), text, underlines, selection_start, selection_end));
}
