std::unique_ptr<TracedValue> InspectorMarkLoadEvent::Data(LocalFrame* frame) {
  std::unique_ptr<TracedValue> frame_data = FrameEventData(frame);
  frame_data->SetString("frame", ToHexString(frame));
  return frame_data;
}
