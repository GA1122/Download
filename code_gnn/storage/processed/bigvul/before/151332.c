std::unique_ptr<TracedValue> InspectorSetLayerTreeId::Data(
    const String& session_id,
    int layer_tree_id) {
  std::unique_ptr<TracedValue> value = TracedValue::Create();
  value->SetString("sessionId", session_id);
  value->SetInteger("layerTreeId", layer_tree_id);
  return value;
}
