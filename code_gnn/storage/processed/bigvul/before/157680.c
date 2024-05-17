bool ImeObserver::ShouldForwardKeyEvent() const {
  const extensions::EventListenerMap::ListenerList& listeners =
      extensions::EventRouter::Get(profile_)
          ->listeners()
          .GetEventListenersByName(input_ime::OnKeyEvent::kEventName);
  for (const std::unique_ptr<extensions::EventListener>& listener : listeners) {
    if (listener->extension_id() == extension_id_ && !listener->IsLazy())
      return true;
  }
  return false;
}
