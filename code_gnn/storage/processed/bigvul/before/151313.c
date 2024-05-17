std::unique_ptr<TracedValue> InspectorCommitLoadEvent::Data(LocalFrame* frame) {
  std::unique_ptr<TracedValue> frame_data = FrameEventData(frame);
  FillCommonFrameData(frame_data.get(), frame);
  return frame_data;
}
