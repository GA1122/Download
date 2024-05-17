void LocalDOMWindow::EnqueuePopstateEvent(
    scoped_refptr<SerializedScriptValue> state_object) {
  DispatchEvent(PopStateEvent::Create(std::move(state_object), history()));
}
