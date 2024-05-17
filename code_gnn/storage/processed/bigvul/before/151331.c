std::unique_ptr<TracedValue> InspectorTracingStartedInFrame::Data(
    const String& session_id,
    LocalFrame* frame) {
  std::unique_ptr<TracedValue> value = TracedValue::Create();
  value->SetString("sessionId", session_id);
  value->SetString("page", ToHexString(&frame->LocalFrameRoot()));
  value->BeginArray("frames");
  for (Frame* f = frame; f; f = f->Tree().TraverseNext(frame)) {
    if (!f->IsLocalFrame())
      continue;
    value->BeginDictionary();
    FillCommonFrameData(value.get(), ToLocalFrame(f));
    value->EndDictionary();
  }
  value->EndArray();
  return value;
}
