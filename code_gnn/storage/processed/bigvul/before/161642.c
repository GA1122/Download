void AudioNode::Trace(blink::Visitor* visitor) {
  visitor->Trace(context_);
  visitor->Trace(connected_nodes_);
  visitor->Trace(connected_params_);
  EventTargetWithInlineData::Trace(visitor);
}
