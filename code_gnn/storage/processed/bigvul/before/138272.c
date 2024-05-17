void AXObjectCacheImpl::postNotification(LayoutObject* layoutObject,
                                         AXNotification notification) {
  if (!layoutObject)
    return;
  postNotification(get(layoutObject), notification);
}
