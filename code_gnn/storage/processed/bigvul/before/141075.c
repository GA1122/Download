HTMLBodyElement* Document::FirstBodyElement() const {
  if (!documentElement() || !IsHTMLHtmlElement(documentElement()))
    return nullptr;

  for (HTMLElement* child =
           Traversal<HTMLElement>::FirstChild(*documentElement());
       child; child = Traversal<HTMLElement>::NextSibling(*child)) {
    if (auto* body = ToHTMLBodyElementOrNull(*child))
      return body;
  }

  return nullptr;
}
