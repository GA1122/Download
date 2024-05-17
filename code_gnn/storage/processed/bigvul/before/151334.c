std::unique_ptr<TracedValue> InspectorAnimationStateEvent::Data(
    const Animation& animation) {
  std::unique_ptr<TracedValue> value = TracedValue::Create();
  value->SetString("state", animation.playState());
  return value;
}
