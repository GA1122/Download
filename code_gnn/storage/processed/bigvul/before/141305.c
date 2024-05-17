HTMLElement* Document::body() const {
  if (!documentElement() || !IsHTMLHtmlElement(documentElement()))
    return nullptr;

  for (HTMLElement* child =
           Traversal<HTMLElement>::FirstChild(*documentElement());
       child; child = Traversal<HTMLElement>::NextSibling(*child)) {
    if (IsHTMLFrameSetElement(*child) || IsHTMLBodyElement(*child))
      return child;
  }

  return nullptr;
}
