void EventBindings::DetachEvent(const std::string& event_name, bool is_manual) {
  attached_event_names_.erase(event_name);

  const std::string& extension_id = context()->GetExtensionID();

  if (DecrementEventListenerCount(context(), event_name) == 0) {
    content::RenderThread::Get()->Send(new ExtensionHostMsg_RemoveListener(
        extension_id, context()->url(), event_name));
  }

  if (is_manual && ExtensionFrameHelper::IsContextForEventPage(context())) {
    content::RenderThread::Get()->Send(
        new ExtensionHostMsg_RemoveLazyListener(extension_id, event_name));
  }
}
