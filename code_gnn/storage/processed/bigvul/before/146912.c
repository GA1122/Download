void Document::setDir(const AtomicString& value) {
  Element* root_element = documentElement();
  if (isHTMLHtmlElement(root_element))
    toHTMLHtmlElement(root_element)->setDir(value);
}
