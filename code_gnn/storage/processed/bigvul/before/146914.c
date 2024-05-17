void Document::setTitle(const String& title) {
  if (!title_element_) {
    if (IsHTMLDocument() || IsXHTMLDocument()) {
      HTMLElement* head_element = head();
      if (!head_element)
        return;
      title_element_ = HTMLTitleElement::Create(*this);
      head_element->AppendChild(title_element_.Get());
    } else if (IsSVGDocument()) {
      Element* element = documentElement();
      if (!isSVGSVGElement(element))
        return;
      title_element_ = SVGTitleElement::Create(*this);
      element->InsertBefore(title_element_.Get(), element->firstChild());
    }
  } else {
    if (!IsHTMLDocument() && !IsXHTMLDocument() && !IsSVGDocument())
      title_element_ = nullptr;
  }

  if (isHTMLTitleElement(title_element_))
    toHTMLTitleElement(title_element_)->setText(title);
  else if (isSVGTitleElement(title_element_))
    toSVGTitleElement(title_element_)->SetText(title);
  else
    UpdateTitle(title);
}
