Element* Document::CreateRawElement(const QualifiedName& qname,
                                    CreateElementFlags flags) {
  Element* element = nullptr;
  if (qname.NamespaceURI() == html_names::xhtmlNamespaceURI) {
    element = HTMLElementFactory::Create(qname.LocalName(), *this, flags);
    if (!element) {
      if (CustomElement::IsValidName(qname.LocalName()))
        element = HTMLElement::Create(qname, *this);
      else
        element = HTMLUnknownElement::Create(qname, *this);
    }
    saw_elements_in_known_namespaces_ = true;
  } else if (qname.NamespaceURI() == svg_names::kNamespaceURI) {
    element = SVGElementFactory::Create(qname.LocalName(), *this, flags);
    if (!element)
      element = SVGUnknownElement::Create(qname, *this);
    saw_elements_in_known_namespaces_ = true;
  } else {
    element = Element::Create(qname, this);
  }

  if (element->prefix() != qname.Prefix())
    element->SetTagNameForCreateElementNS(qname);
  DCHECK(qname == element->TagQName());

  return element;
}
