void LocalDOMWindow::EnqueueHashchangeEvent(const String& old_url,
                                            const String& new_url) {
  EnqueueWindowEvent(HashChangeEvent::Create(old_url, new_url));
}
