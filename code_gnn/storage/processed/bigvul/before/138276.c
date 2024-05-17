void AXObjectCacheImpl::radiobuttonRemovedFromGroup(
    HTMLInputElement* groupMember) {
  AXObject* obj = get(groupMember);
  if (!obj || !obj->isAXRadioInput())
    return;

  HTMLInputElement* firstRadio =
      toAXRadioInput(obj)->findFirstRadioButtonInGroup(groupMember);
  AXObject* firstObj = get(firstRadio);
  if (!firstObj || !firstObj->isAXRadioInput())
    return;

  toAXRadioInput(firstObj)->updatePosAndSetSize(1);
  postNotification(firstObj, AXAriaAttributeChanged);
  toAXRadioInput(firstObj)->requestUpdateToNextNode(true);
}
