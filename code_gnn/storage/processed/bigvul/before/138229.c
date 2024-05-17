void AXObjectCacheImpl::dispose() {
  m_notificationPostTimer.stop();

  for (auto& entry : m_objects) {
    AXObject* obj = entry.value;
    obj->detach();
    removeAXID(obj);
  }

#if DCHECK_IS_ON()
  m_hasBeenDisposed = true;
#endif
}
