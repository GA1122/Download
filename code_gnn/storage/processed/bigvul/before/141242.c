void Document::SetTitleElement(Element* title_element) {
  if (IsSVGSVGElement(documentElement())) {
    title_element_ = Traversal<SVGTitleElement>::FirstChild(*documentElement());
  } else {
    if (title_element_ && title_element_ != title_element)
      title_element_ = Traversal<HTMLTitleElement>::FirstWithin(*this);
    else
      title_element_ = title_element;

    if (IsSVGTitleElement(title_element_)) {
      title_element_ = nullptr;
      return;
    }
  }

  if (auto* html_title = ToHTMLTitleElementOrNull(title_element_))
    UpdateTitle(html_title->text());
  else if (auto* svg_title = ToSVGTitleElementOrNull(title_element_))
    UpdateTitle(svg_title->textContent());
}
