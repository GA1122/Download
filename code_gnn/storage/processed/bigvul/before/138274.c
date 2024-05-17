void AXObjectCacheImpl::postNotification(AXObject* object,
                                         AXNotification notification) {
  if (!object)
    return;

  m_modificationCount++;
  m_notificationsToPost.push_back(std::make_pair(object, notification));
  if (!m_notificationPostTimer.isActive())
    m_notificationPostTimer.startOneShot(0, BLINK_FROM_HERE);
}
