HTMLElement* Document::body() const {
  if (!documentElement() || !isHTMLHtmlElement(documentElement()))
    return 0;

  for (HTMLElement* child =
           Traversal<HTMLElement>::FirstChild(*documentElement());
       child; child = Traversal<HTMLElement>::NextSibling(*child)) {
    if (isHTMLFrameSetElement(*child) || isHTMLBodyElement(*child))
      return child;
  }

  return 0;
}
