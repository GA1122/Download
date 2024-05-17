String AXObject::language() const {
  const AtomicString& lang = getAttribute(langAttr);
  if (!lang.isEmpty())
    return lang;

  AXObject* parent = parentObject();

  if (!parent) {
    Document* doc = getDocument();
    if (doc)
      return doc->contentLanguage();
    return nullAtom;
  }

  return parent->language();
}
