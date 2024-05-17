Element* HTMLFormElement::elementFromPastNamesMap(
    const AtomicString& pastName) {
  if (pastName.isEmpty() || !m_pastNamesMap)
    return 0;
  Element* element = m_pastNamesMap->get(pastName);
#if DCHECK_IS_ON()
  if (!element)
    return 0;
  SECURITY_DCHECK(toHTMLElement(element)->formOwner() == this);
  if (isHTMLImageElement(*element)) {
    SECURITY_DCHECK(imageElements().find(element) != kNotFound);
  } else if (isHTMLObjectElement(*element)) {
    SECURITY_DCHECK(associatedElements().find(toHTMLObjectElement(element)) !=
                    kNotFound);
  } else {
    SECURITY_DCHECK(associatedElements().find(
                        toHTMLFormControlElement(element)) != kNotFound);
  }
#endif
  return element;
}
