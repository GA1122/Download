void AXLayoutObject::detach() {
  AXNodeObject::detach();

  detachRemoteSVGRoot();

#if DCHECK_IS_ON()
  if (m_layoutObject)
    m_layoutObject->setHasAXObject(false);
#endif
  m_layoutObject = 0;
}
