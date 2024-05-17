void HTMLFormElement::didAssociateByParser() {
  if (!m_didFinishParsingChildren)
    return;
  m_hasElementsAssociatedByParser = true;
  UseCounter::count(document(), UseCounter::FormAssociationByParser);
}
