void Document::RemoveTitle(Element* title_element) {
  if (title_element_ != title_element)
    return;

  title_element_ = nullptr;

  if (IsHTMLDocument() || IsXHTMLDocument()) {
    if (HTMLTitleElement* title =
            Traversal<HTMLTitleElement>::FirstWithin(*this))
      SetTitleElement(title);
  } else if (IsSVGDocument()) {
    if (SVGTitleElement* title = Traversal<SVGTitleElement>::FirstWithin(*this))
      SetTitleElement(title);
  }

  if (!title_element_)
    UpdateTitle(String());
}
