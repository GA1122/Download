void AXObjectCacheImpl::updateTreeIfElementIdIsAriaOwned(Element* element) {
  if (!element->hasID())
    return;

  String id = element->getIdAttribute();
  HashSet<AXID>* owners = m_idToAriaOwnersMapping.at(id);
  if (!owners)
    return;

  AXObject* axElement = getOrCreate(element);
  if (!axElement)
    return;

  if (isAriaOwned(axElement)) {
    AXObject* ownedParent = getAriaOwnedParent(axElement);
    DCHECK(ownedParent);
    childrenChanged(ownedParent);
    return;
  }

  for (const auto& axID : *owners) {
    AXObject* owner = objectFromAXID(axID);
    if (owner)
      childrenChanged(owner);
  }
}
