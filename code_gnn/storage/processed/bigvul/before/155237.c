void HTMLFormElement::parseAttribute(const QualifiedName& name,
                                     const AtomicString& oldValue,
                                     const AtomicString& value) {
  if (name == actionAttr) {
    m_attributes.parseAction(value);
    logUpdateAttributeIfIsolatedWorldAndInDocument("form", actionAttr, oldValue,
                                                   value);

    if (document().getInsecureRequestPolicy() & kUpgradeInsecureRequests)
      return;
    KURL actionURL = document().completeURL(m_attributes.action().isEmpty()
                                                ? document().url().getString()
                                                : m_attributes.action());
    if (MixedContentChecker::isMixedFormAction(document().frame(), actionURL))
      UseCounter::count(document().frame(),
                        UseCounter::MixedContentFormPresent);
  } else if (name == targetAttr) {
    m_attributes.setTarget(value);
  } else if (name == methodAttr) {
    m_attributes.updateMethodType(value);
  } else if (name == enctypeAttr) {
    m_attributes.updateEncodingType(value);
  } else if (name == accept_charsetAttr) {
    m_attributes.setAcceptCharset(value);
  } else {
    HTMLElement::parseAttribute(name, oldValue, value);
  }
}
