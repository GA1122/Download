HTMLBodyElement* Document::FirstBodyElement() const {
  if (!documentElement() || !isHTMLHtmlElement(documentElement()))
    return 0;

  for (HTMLElement* child =
           Traversal<HTMLElement>::FirstChild(*documentElement());
       child; child = Traversal<HTMLElement>::NextSibling(*child)) {
    if (isHTMLBodyElement(*child))
      return toHTMLBodyElement(child);
  }

  return 0;
}
