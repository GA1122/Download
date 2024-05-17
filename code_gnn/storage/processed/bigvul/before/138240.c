AXObject* AXObjectCacheImpl::getOrCreate(AbstractInlineTextBox* inlineTextBox) {
  if (!inlineTextBox)
    return 0;

  if (AXObject* obj = get(inlineTextBox))
    return obj;

  AXObject* newObj = createFromInlineTextBox(inlineTextBox);

  DCHECK(!get(inlineTextBox));

  const AXID axid = getOrCreateAXID(newObj);

  m_inlineTextBoxObjectMapping.set(inlineTextBox, axid);
  newObj->init();
  newObj->setLastKnownIsIgnoredValue(newObj->accessibilityIsIgnored());

  return newObj;
}
