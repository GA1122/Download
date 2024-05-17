std::unique_ptr<TracedValue> InspectorAnimationEvent::Data(
    const Animation& animation) {
  std::unique_ptr<TracedValue> value = TracedValue::Create();
  value->SetString("id", String::Number(animation.SequenceNumber()));
  value->SetString("state", animation.playState());
  if (const AnimationEffectReadOnly* effect = animation.effect()) {
    value->SetString("name", animation.id());
    if (effect->IsKeyframeEffectReadOnly()) {
      if (Element* target = ToKeyframeEffectReadOnly(effect)->Target())
        SetNodeInfo(value.get(), target, "nodeId", "nodeName");
    }
  }
  return value;
}
