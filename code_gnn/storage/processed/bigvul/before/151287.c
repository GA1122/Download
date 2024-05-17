std::unique_ptr<TracedValue> InspectorLayoutEvent::BeginData(
    LocalFrameView* frame_view) {
  bool is_partial;
  unsigned needs_layout_objects;
  unsigned total_objects;
  LocalFrame& frame = frame_view->GetFrame();
  frame.View()->CountObjectsNeedingLayout(needs_layout_objects, total_objects,
                                          is_partial);

  std::unique_ptr<TracedValue> value = TracedValue::Create();
  value->SetInteger("dirtyObjects", needs_layout_objects);
  value->SetInteger("totalObjects", total_objects);
  value->SetBoolean("partialLayout", is_partial);
  value->SetString("frame", ToHexString(&frame));
  SetCallStack(value.get());
  return value;
}
