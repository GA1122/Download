bool HTMLFormControlElement::recalcWillValidate() const {
  if (m_dataListAncestorState == Unknown) {
    if (Traversal<HTMLDataListElement>::firstAncestor(*this))
      m_dataListAncestorState = InsideDataList;
    else
      m_dataListAncestorState = NotInsideDataList;
  }
  return m_dataListAncestorState == NotInsideDataList &&
         !isDisabledOrReadOnly();
}
