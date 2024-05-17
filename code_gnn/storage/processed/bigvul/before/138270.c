void AXObjectCacheImpl::notificationPostTimerFired(TimerBase*) {
  m_notificationPostTimer.stop();

  unsigned i = 0, count = m_notificationsToPost.size();
  for (i = 0; i < count; ++i) {
    AXObject* obj = m_notificationsToPost[i].first;

    if (!obj->axObjectID())
      continue;

    if (obj->isDetached())
      continue;

#if DCHECK_IS_ON()
    if (obj->isAXLayoutObject()) {
      AXLayoutObject* layoutObj = toAXLayoutObject(obj);
      LayoutObject* layoutObject = layoutObj->getLayoutObject();
      if (layoutObject && layoutObject->view())
        DCHECK(!layoutObject->view()->layoutState());
    }
#endif

    AXNotification notification = m_notificationsToPost[i].second;
    postPlatformNotification(obj, notification);

    if (notification == AXChildrenChanged && obj->parentObjectIfExists() &&
        obj->lastKnownIsIgnoredValue() != obj->accessibilityIsIgnored())
      childrenChanged(obj->parentObject());
  }

  m_notificationsToPost.clear();
}
