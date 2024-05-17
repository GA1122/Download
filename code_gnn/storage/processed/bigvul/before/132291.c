void RenderFrameImpl::didAccessInitialDocument(blink::WebLocalFrame* frame) {
  DCHECK(!frame_ || frame_ == frame);
  if (!frame->parent()) {
    DocumentState* document_state =
        DocumentState::FromDataSource(frame->dataSource());
    NavigationStateImpl* navigation_state =
        static_cast<NavigationStateImpl*>(document_state->navigation_state());

    if (!navigation_state->request_committed()) {
      Send(new FrameHostMsg_DidAccessInitialDocument(routing_id_));
    }
  }
}
