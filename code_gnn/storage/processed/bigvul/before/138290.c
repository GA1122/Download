void AXObjectCacheImpl::updateAriaOwns(
    const AXObject* owner,
    const Vector<String>& idVector,
    HeapVector<Member<AXObject>>& ownedChildren) {

  HashSet<String> currentIds = m_ariaOwnerToIdsMapping.at(owner->axObjectID());
  HashSet<String> newIds;
  bool idsChanged = false;
  for (const String& id : idVector) {
    newIds.insert(id);
    if (!currentIds.contains(id)) {
      idsChanged = true;
      HashSet<AXID>* owners = m_idToAriaOwnersMapping.at(id);
      if (!owners) {
        owners = new HashSet<AXID>();
        m_idToAriaOwnersMapping.set(id, WTF::wrapUnique(owners));
      }
      owners->insert(owner->axObjectID());
    }
  }
  for (const String& id : currentIds) {
    if (!newIds.contains(id)) {
      idsChanged = true;
      HashSet<AXID>* owners = m_idToAriaOwnersMapping.at(id);
      if (owners) {
        owners->erase(owner->axObjectID());
        if (owners->isEmpty())
          m_idToAriaOwnersMapping.erase(id);
      }
    }
  }
  if (idsChanged)
    m_ariaOwnerToIdsMapping.set(owner->axObjectID(), newIds);


  TreeScope& scope = owner->getNode()->treeScope();
  Vector<AXID> newChildAXIDs;
  for (const String& idName : idVector) {
    Element* element = scope.getElementById(AtomicString(idName));
    if (!element)
      continue;

    AXObject* child = getOrCreate(element);
    if (!child)
      continue;

    if (isAriaOwned(child) && getAriaOwnedParent(child) != owner)
      continue;

    if (child == owner)
      continue;

    bool foundCycle = false;
    for (AXObject* parent = owner->parentObject(); parent && !foundCycle;
         parent = parent->parentObject()) {
      if (parent == child)
        foundCycle = true;
    }
    if (foundCycle)
      continue;

    newChildAXIDs.push_back(child->axObjectID());
    ownedChildren.push_back(child);
  }

  Vector<AXID> currentChildAXIDs =
      m_ariaOwnerToChildrenMapping.at(owner->axObjectID());
  bool same = true;
  if (currentChildAXIDs.size() != newChildAXIDs.size()) {
    same = false;
  } else {
    for (size_t i = 0; i < currentChildAXIDs.size() && same; ++i) {
      if (currentChildAXIDs[i] != newChildAXIDs[i])
        same = false;
    }
  }
  if (same)
    return;

  for (size_t i = 0; i < currentChildAXIDs.size(); ++i) {
    AXID removedChildID = currentChildAXIDs[i];
    AXObject* removedChild = objectFromAXID(removedChildID);

    if (removedChild && getAriaOwnedParent(removedChild) != owner)
      continue;

    m_ariaOwnedChildToOwnerMapping.erase(removedChildID);

    if (removedChild) {
      removedChild->detachFromParent();
      AXID realParentID =
          m_ariaOwnedChildToRealParentMapping.at(removedChildID);
      AXObject* realParent = objectFromAXID(realParentID);
      childrenChanged(realParent);
    }

    m_ariaOwnedChildToRealParentMapping.erase(removedChildID);
  }

  for (size_t i = 0; i < newChildAXIDs.size(); ++i) {
    AXID addedChildID = newChildAXIDs[i];
    AXObject* addedChild = objectFromAXID(addedChildID);

    m_ariaOwnedChildToOwnerMapping.set(addedChildID, owner->axObjectID());

    AXObject* originalParent = addedChild->parentObject();
    m_ariaOwnedChildToRealParentMapping.set(addedChildID,
                                            originalParent->axObjectID());

    addedChild->detachFromParent();
    childrenChanged(originalParent);
  }

  m_ariaOwnerToChildrenMapping.set(owner->axObjectID(), newChildAXIDs);
}
