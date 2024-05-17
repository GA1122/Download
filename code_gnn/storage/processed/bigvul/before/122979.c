void RenderWidgetHostImpl::ImeCancelComposition() {
  Send(new ViewMsg_ImeSetComposition(GetRoutingID(), string16(),
            std::vector<WebKit::WebCompositionUnderline>(), 0, 0));
}
