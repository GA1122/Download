AXObject* AXLayoutObject::accessibilityHitTest(const IntPoint& point) const {
  if (!m_layoutObject || !m_layoutObject->hasLayer())
    return nullptr;

  PaintLayer* layer = toLayoutBox(m_layoutObject)->layer();

  HitTestRequest request(HitTestRequest::ReadOnly | HitTestRequest::Active);
  HitTestResult hitTestResult = HitTestResult(request, point);
  layer->hitTest(hitTestResult);

  Node* node = hitTestResult.innerNode();
  if (!node)
    return nullptr;

  if (isHTMLAreaElement(node))
    return accessibilityImageMapHitTest(toHTMLAreaElement(node), point);

  if (isHTMLOptionElement(node)) {
    node = toHTMLOptionElement(*node).ownerSelectElement();
    if (!node)
      return nullptr;
  }

  LayoutObject* obj = node->layoutObject();
  if (!obj)
    return nullptr;

  AXObject* result = axObjectCache().getOrCreate(obj);
  result->updateChildrenIfNecessary();

  result = result->elementAccessibilityHitTest(point);
  if (result && result->accessibilityIsIgnored()) {
    if (result->isAXLayoutObject()) {
      AXObject* controlObject =
          toAXLayoutObject(result)->correspondingControlForLabelElement();
      if (controlObject && controlObject->nameFromLabelElement())
        return controlObject;
    }

    result = result->parentObjectUnignored();
  }

  return result;
}
