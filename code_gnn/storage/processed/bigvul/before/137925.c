AXObject* AXLayoutObject::computeParent() const {
  ASSERT(!isDetached());
  if (!m_layoutObject)
    return 0;

  if (ariaRoleAttribute() == MenuBarRole)
    return axObjectCache().getOrCreate(m_layoutObject->parent());

  if (ariaRoleAttribute() == MenuRole) {
    AXObject* parent = menuButtonForMenu();
    if (parent)
      return parent;
  }

  LayoutObject* parentObj = layoutParentObject();
  if (parentObj)
    return axObjectCache().getOrCreate(parentObj);

  if (isWebArea()) {
    LocalFrame* frame = m_layoutObject->frame();
    return axObjectCache().getOrCreate(frame->pagePopupOwner());
  }

  return 0;
}
