void LocalDOMWindow::StatePopped(
    scoped_refptr<SerializedScriptValue> state_object) {
  if (!GetFrame())
    return;

  if (document()->IsLoadCompleted())
    EnqueuePopstateEvent(std::move(state_object));
  else
    pending_state_object_ = std::move(state_object);
}
