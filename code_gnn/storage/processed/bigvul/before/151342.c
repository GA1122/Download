void InspectorTraceEvents::DidReceiveData(unsigned long identifier,
                                          DocumentLoader* loader,
                                          const char* data,
                                          int encoded_data_length) {
  LocalFrame* frame = loader ? loader->GetFrame() : nullptr;
  TRACE_EVENT_INSTANT1(
      "devtools.timeline", "ResourceReceivedData", TRACE_EVENT_SCOPE_THREAD,
      "data",
      InspectorReceiveDataEvent::Data(identifier, frame, encoded_data_length));
  probe::AsyncTask async_task(frame ? frame->GetDocument() : nullptr,
                              AsyncId(identifier), "data");
}
