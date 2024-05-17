AXObject* AXObjectCacheImpl::get(AbstractInlineTextBox* inlineTextBox) {
  if (!inlineTextBox)
    return 0;

  AXID axID = m_inlineTextBoxObjectMapping.at(inlineTextBox);
  DCHECK(!HashTraits<AXID>::isDeletedValue(axID));
  if (!axID)
    return 0;

  return m_objects.at(axID);
}
