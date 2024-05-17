void BaseAudioContext::Trace(blink::Visitor* visitor) {
  visitor->Trace(destination_node_);
  visitor->Trace(listener_);
  visitor->Trace(active_source_nodes_);
  visitor->Trace(resume_resolvers_);
  visitor->Trace(decode_audio_resolvers_);
  visitor->Trace(periodic_wave_sine_);
  visitor->Trace(periodic_wave_square_);
  visitor->Trace(periodic_wave_sawtooth_);
  visitor->Trace(periodic_wave_triangle_);
  visitor->Trace(audio_worklet_);
  EventTargetWithInlineData::Trace(visitor);
  PausableObject::Trace(visitor);
}
