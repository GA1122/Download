void FrameFetchContext::DispatchDidFinishLoading(unsigned long identifier,
                                                 double finish_time,
                                                 int64_t encoded_data_length,
                                                 int64_t decoded_body_length) {
  if (IsDetached())
    return;

  GetFrame()->Loader().Progress().CompleteProgress(identifier);
  probe::didFinishLoading(GetFrame()->GetDocument(), identifier,
                          MasterDocumentLoader(), finish_time,
                          encoded_data_length, decoded_body_length);
  if (GetFrame()->FrameScheduler())
    GetFrame()->FrameScheduler()->DidStopLoading(identifier);
}
