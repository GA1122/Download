void AXObjectCacheImpl::remove(AbstractInlineTextBox* inlineTextBox) {
  if (!inlineTextBox)
    return;

  AXID axID = m_inlineTextBoxObjectMapping.at(inlineTextBox);
  remove(axID);
  m_inlineTextBoxObjectMapping.erase(inlineTextBox);
}
