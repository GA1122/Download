void HTMLFormElement::addToPastNamesMap(Element* element,
                                        const AtomicString& pastName) {
  if (pastName.isEmpty())
    return;
  if (!m_pastNamesMap)
    m_pastNamesMap = new PastNamesMap;
  m_pastNamesMap->set(pastName, element);
}
