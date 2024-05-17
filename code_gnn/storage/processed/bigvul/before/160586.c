void RenderFrameImpl::DidFinishDocumentLoad() {
  TRACE_EVENT1("navigation,benchmark,rail",
               "RenderFrameImpl::didFinishDocumentLoad", "id", routing_id_);
  Send(new FrameHostMsg_DidFinishDocumentLoad(routing_id_));

  {
    SCOPED_UMA_HISTOGRAM_TIMER("RenderFrameObservers.DidFinishDocumentLoad");
    for (auto& observer : observers_)
      observer.DidFinishDocumentLoad();
  }

  UpdateEncoding(frame_, frame_->View()->PageEncoding().Utf8());
}
