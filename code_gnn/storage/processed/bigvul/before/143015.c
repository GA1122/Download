void BaseAudioContext::NotifyStateChange() {
  DispatchEvent(*Event::Create(event_type_names::kStatechange));
}
