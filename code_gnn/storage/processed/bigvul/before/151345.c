std::unique_ptr<TracedValue> InspectorLayoutEvent::EndData(
    LayoutObject* root_for_this_layout) {
  Vector<FloatQuad> quads;
  root_for_this_layout->AbsoluteQuads(quads);

  std::unique_ptr<TracedValue> value = TracedValue::Create();
  if (quads.size() >= 1) {
    CreateQuad(value.get(), "root", quads[0]);
    SetGeneratingNodeInfo(value.get(), root_for_this_layout, "rootNode");
  } else {
    NOTREACHED();
  }
  return value;
}
