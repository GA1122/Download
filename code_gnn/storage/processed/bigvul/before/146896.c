const AtomicString& Document::dir() {
  Element* root_element = documentElement();
  if (isHTMLHtmlElement(root_element))
    return toHTMLHtmlElement(root_element)->dir();
  return g_null_atom;
}
