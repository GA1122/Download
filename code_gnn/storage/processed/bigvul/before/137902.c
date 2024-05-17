AXLayoutObject::AXLayoutObject(LayoutObject* layoutObject,
                               AXObjectCacheImpl& axObjectCache)
    : AXNodeObject(layoutObject->node(), axObjectCache),
      m_layoutObject(layoutObject) {
#if DCHECK_IS_ON()
  m_layoutObject->setHasAXObject(true);
#endif
}
