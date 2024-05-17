String SVGElement::title() const {
  if (IsSVGSVGElement(*this) && this == GetDocument().documentElement())
    return String();

  if (InUseShadowTree()) {
    String use_title(OwnerShadowHost()->title());
    if (!use_title.IsEmpty())
      return use_title;
  }

  if (Element* title_element = Traversal<SVGTitleElement>::FirstChild(*this))
    return title_element->innerText();

  return String();
}
