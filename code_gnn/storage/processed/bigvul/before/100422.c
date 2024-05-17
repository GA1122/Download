void RenderThread::OnDOMStorageEvent(const string16& key,
    const NullableString16& old_value, const NullableString16& new_value,
    const string16& origin, DOMStorageType dom_storage_type) {
  if (!dom_storage_event_dispatcher_.get()) {
    dom_storage_event_dispatcher_.reset(WebStorageEventDispatcher::create());
  }
  dom_storage_event_dispatcher_->dispatchStorageEvent(key, old_value, new_value,
      origin, dom_storage_type == DOM_STORAGE_LOCAL);
}
