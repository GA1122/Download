const FormAssociatedElement::List& HTMLFormElement::associatedElements() const {
  if (!m_associatedElementsAreDirty)
    return m_associatedElements;
  HTMLFormElement* mutableThis = const_cast<HTMLFormElement*>(this);
  Node* scope = mutableThis;
  if (m_hasElementsAssociatedByParser)
    scope = &NodeTraversal::highestAncestorOrSelf(*mutableThis);
  if (isConnected() && m_hasElementsAssociatedByFormAttribute)
    scope = &treeScope().rootNode();
  DCHECK(scope);
  collectAssociatedElements(*scope, mutableThis->m_associatedElements);
  mutableThis->m_associatedElementsAreDirty = false;
  return m_associatedElements;
}
