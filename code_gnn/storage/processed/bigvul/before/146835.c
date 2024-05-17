void Document::SetTitleElement(Element* title_element) {
  if (isSVGSVGElement(documentElement())) {
    title_element_ = Traversal<SVGTitleElement>::FirstChild(*documentElement());
  } else {
    if (title_element_ && title_element_ != title_element)
      title_element_ = Traversal<HTMLTitleElement>::FirstWithin(*this);
    else
      title_element_ = title_element;

    if (isSVGTitleElement(title_element_)) {
      title_element_ = nullptr;
      return;
    }
  }

  if (isHTMLTitleElement(title_element_))
    UpdateTitle(toHTMLTitleElement(title_element_)->text());
  else if (isSVGTitleElement(title_element_))
    UpdateTitle(toSVGTitleElement(title_element_)->textContent());
}
