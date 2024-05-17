void Document::setDir(const AtomicString& value) {
  Element* root_element = documentElement();
  if (auto* html = ToHTMLHtmlElementOrNull(root_element))
    html->setDir(value);
}
