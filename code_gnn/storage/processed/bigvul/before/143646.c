void RenderWidgetHostImpl::ImeCancelComposition() {
  Send(new InputMsg_ImeSetComposition(GetRoutingID(), base::string16(),
            std::vector<blink::WebCompositionUnderline>(),
            gfx::Range::InvalidRange(), 0, 0));
}
