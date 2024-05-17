void FrameFetchContext::AddResourceTiming(const ResourceTimingInfo& info) {
  Document* initiator_document = document_ && info.IsMainResource()
                                     ? document_->ParentDocument()
                                     : document_.Get();
  if (!initiator_document || !initiator_document->domWindow())
    return;
  DOMWindowPerformance::performance(*initiator_document->domWindow())
      ->AddResourceTiming(info);
}
