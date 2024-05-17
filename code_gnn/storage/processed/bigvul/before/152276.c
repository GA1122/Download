void RenderFrameImpl::DidFinishLoad() {
  TRACE_EVENT1("navigation,benchmark,rail",
               "RenderFrameImpl::didFinishLoad", "id", routing_id_);
  if (!frame_->Parent()) {
    TRACE_EVENT_INSTANT0("WebCore,benchmark,rail", "LoadFinished",
                         TRACE_EVENT_SCOPE_PROCESS);
  }

  for (auto& observer : observers_)
    observer.DidFinishLoad();

  WebDocumentLoader* document_loader = frame_->GetDocumentLoader();
  Send(new FrameHostMsg_DidFinishLoad(routing_id_, document_loader->GetUrl()));

  if (!RenderThreadImpl::current())
    return;
  RenderThreadImpl::RendererMemoryMetrics memory_metrics;
  if (!RenderThreadImpl::current()->GetRendererMemoryMetrics(&memory_metrics))
    return;
  RecordSuffixedRendererMemoryMetrics(memory_metrics, ".DidFinishLoad");
  if (!IsMainFrame())
    return;
  RecordSuffixedRendererMemoryMetrics(memory_metrics,
                                      ".MainFrameDidFinishLoad");
}