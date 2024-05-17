void RenderFrameImpl::didFinishDocumentLoad(blink::WebLocalFrame* frame,
                                            bool document_is_empty) {
  TRACE_EVENT1("navigation", "RenderFrameImpl::didFinishDocumentLoad",
               "id", routing_id_);
  DCHECK(!frame_ || frame_ == frame);
  WebDataSource* ds = frame->dataSource();
  DocumentState* document_state = DocumentState::FromDataSource(ds);
  document_state->set_finish_document_load_time(Time::Now());

  Send(new FrameHostMsg_DidFinishDocumentLoad(routing_id_));

  FOR_EACH_OBSERVER(RenderViewObserver, render_view_->observers(),
                    DidFinishDocumentLoad(frame));
  FOR_EACH_OBSERVER(RenderFrameObserver, observers_, DidFinishDocumentLoad());

  UpdateEncoding(frame, frame->view()->pageEncoding().utf8());

  if (!document_is_empty)
    return;

  RenderFrameImpl* localRoot = this;
  while (localRoot->frame_ && localRoot->frame_->parent() &&
         localRoot->frame_->parent()->isWebLocalFrame()) {
    localRoot = RenderFrameImpl::FromWebFrame(localRoot->frame_->parent());
    DCHECK(localRoot);
  }
  if (localRoot->devtools_agent_ && localRoot->devtools_agent_->IsAttached())
    return;

  std::string error_domain = "http";
  InternalDocumentStateData* internal_data =
      InternalDocumentStateData::FromDataSource(frame->dataSource());
  int http_status_code = internal_data->http_status_code();
  if (GetContentClient()->renderer()->HasErrorPage(http_status_code,
                                                   &error_domain)) {
    WebURLError error;
    error.unreachableURL = frame->document().url();
    error.domain = WebString::fromUTF8(error_domain);
    error.reason = http_status_code;
    LoadNavigationErrorPage(frame->dataSource()->request(), error, true);
  }
}
