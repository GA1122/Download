void FrameFetchContext::DispatchDidFinishLoading(
    unsigned long identifier,
    TimeTicks finish_time,
    int64_t encoded_data_length,
    int64_t decoded_body_length,
    bool should_report_corb_blocking) {
  if (IsDetached())
    return;

  GetFrame()->Loader().Progress().CompleteProgress(identifier);
  probe::didFinishLoading(GetFrame()->GetDocument(), identifier,
                          MasterDocumentLoader(), finish_time,
                          encoded_data_length, decoded_body_length,
                          should_report_corb_blocking);
  if (document_) {
    InteractiveDetector* interactive_detector(
        InteractiveDetector::From(*document_));
    if (interactive_detector) {
      interactive_detector->OnResourceLoadEnd(finish_time);
    }
  }
}
