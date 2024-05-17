std::unique_ptr<TracedValue> FrameEventData(LocalFrame* frame) {
  std::unique_ptr<TracedValue> value = TracedValue::Create();
  bool is_main_frame = frame && frame->IsMainFrame();
  value->SetBoolean("isMainFrame", is_main_frame);
  value->SetString("page", ToHexString(&frame->LocalFrameRoot()));
  return value;
}
