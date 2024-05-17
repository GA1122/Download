void AXLayoutObject::handleAriaExpandedChanged() {
  AXObject* containerParent = this->parentObject();
  while (containerParent) {
    bool foundParent = false;

    switch (containerParent->roleValue()) {
      case TreeRole:
      case TreeGridRole:
      case GridRole:
      case TableRole:
        foundParent = true;
        break;
      default:
        break;
    }

    if (foundParent)
      break;

    containerParent = containerParent->parentObject();
  }

  if (containerParent)
    axObjectCache().postNotification(containerParent,
                                     AXObjectCacheImpl::AXRowCountChanged);

  AccessibilityExpanded expanded = isExpanded();
  if (!expanded)
    return;

  if (roleValue() == RowRole || roleValue() == TreeItemRole) {
    AXObjectCacheImpl::AXNotification notification =
        AXObjectCacheImpl::AXRowExpanded;
    if (expanded == ExpandedCollapsed)
      notification = AXObjectCacheImpl::AXRowCollapsed;

    axObjectCache().postNotification(this, notification);
  } else {
    axObjectCache().postNotification(this,
                                     AXObjectCacheImpl::AXExpandedChanged);
  }
}
