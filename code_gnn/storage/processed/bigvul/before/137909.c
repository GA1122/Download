void AXLayoutObject::addImageMapChildren() {
  LayoutBoxModelObject* cssBox = getLayoutBoxModelObject();
  if (!cssBox || !cssBox->isLayoutImage())
    return;

  HTMLMapElement* map = toLayoutImage(cssBox)->imageMap();
  if (!map)
    return;

  for (HTMLAreaElement& area :
       Traversal<HTMLAreaElement>::descendantsOf(*map)) {
    AXObject* obj = axObjectCache().getOrCreate(&area);
    if (obj) {
      AXImageMapLink* areaObject = toAXImageMapLink(obj);
      areaObject->setParent(this);
      ASSERT(areaObject->axObjectID() != 0);
      if (!areaObject->accessibilityIsIgnored())
        m_children.push_back(areaObject);
      else
        axObjectCache().remove(areaObject->axObjectID());
    }
  }
}
