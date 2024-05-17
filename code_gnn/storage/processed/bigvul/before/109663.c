void Document::enqueuePopstateEvent(PassRefPtr<SerializedScriptValue> stateObject)
{
    if (!ContextFeatures::pushStateEnabled(this))
        return;

    dispatchWindowEvent(PopStateEvent::create(stateObject, domWindow() ? domWindow()->history() : 0));
}
