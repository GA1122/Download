LayoutObject* AXLayoutObject::layoutParentObject() const {
  if (!m_layoutObject)
    return 0;

  LayoutObject* startOfConts = m_layoutObject->isLayoutBlockFlow()
                                   ? startOfContinuations(m_layoutObject)
                                   : nullptr;
  if (startOfConts) {
    return startOfConts;
  }

  LayoutObject* parent = m_layoutObject->parent();
  startOfConts =
      parent && parent->isLayoutInline() ? startOfContinuations(parent) : 0;
  if (startOfConts) {
    return startOfConts;
  }

  LayoutObject* firstChild = parent ? parent->slowFirstChild() : 0;
  if (firstChild && firstChild->node()) {
    for (LayoutObject* nodeLayoutFirstChild =
             firstChild->node()->layoutObject();
         nodeLayoutFirstChild != firstChild;
         nodeLayoutFirstChild = firstChild->node()->layoutObject()) {
      for (LayoutObject* contsTest = nodeLayoutFirstChild; contsTest;
           contsTest = nextContinuation(contsTest)) {
        if (contsTest == firstChild) {
          parent = nodeLayoutFirstChild->parent();
          break;
        }
      }
      LayoutObject* newFirstChild = parent->slowFirstChild();
      if (firstChild == newFirstChild)
        break;
      firstChild = newFirstChild;
      if (!firstChild->node())
        break;
    }
  }

  return parent;
}
