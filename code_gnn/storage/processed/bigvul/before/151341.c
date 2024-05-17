void InspectorTraceEvents::DidFinishLoading(unsigned long identifier,
                                            DocumentLoader* loader,
                                            double finish_time,
                                            int64_t encoded_data_length,
                                            int64_t decoded_body_length) {
  LocalFrame* frame = loader ? loader->GetFrame() : nullptr;
  TRACE_EVENT_INSTANT1("devtools.timeline", "ResourceFinish",
                       TRACE_EVENT_SCOPE_THREAD, "data",
                       InspectorResourceFinishEvent::Data(
                           identifier, finish_time, false, encoded_data_length,
                           decoded_body_length));
  probe::AsyncTask async_task(frame ? frame->GetDocument() : nullptr,
                              AsyncId(identifier));
}
