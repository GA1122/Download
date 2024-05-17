Element* Document::createElement(const QualifiedName& q_name,
                                 CreateElementFlags flags) {
  Element* e = nullptr;

  if (q_name.NamespaceURI() == xhtmlNamespaceURI)
    e = HTMLElementFactory::createHTMLElement(q_name.LocalName(), *this, flags);
  else if (q_name.NamespaceURI() == SVGNames::svgNamespaceURI)
    e = SVGElementFactory::createSVGElement(q_name.LocalName(), *this, flags);

  if (e)
    saw_elements_in_known_namespaces_ = true;
  else
    e = Element::Create(q_name, this);

  if (e->prefix() != q_name.Prefix())
    e->SetTagNameForCreateElementNS(q_name);

  DCHECK(q_name == e->TagQName());

  return e;
}
