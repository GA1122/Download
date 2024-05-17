void BaseAudioContext::NotifyStateChange() {
  DispatchEvent(*Event::Create(EventTypeNames::statechange));
}
