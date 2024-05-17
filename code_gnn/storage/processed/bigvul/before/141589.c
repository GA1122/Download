void EventBindings::AttachEvent(const std::string& event_name) {
  if (!context()->HasAccessOrThrowError(event_name))
    return;

  attached_event_names_.insert(event_name);

  const std::string& extension_id = context()->GetExtensionID();
  if (IncrementEventListenerCount(context(), event_name) == 1) {
    content::RenderThread::Get()->Send(new ExtensionHostMsg_AddListener(
        extension_id, context()->url(), event_name));
  }

  if (ExtensionFrameHelper::IsContextForEventPage(context())) {
    content::RenderThread::Get()->Send(
        new ExtensionHostMsg_AddLazyListener(extension_id, event_name));
  }
}
