bool HTMLIFrameElement::IsPresentationAttribute(
    const QualifiedName& name) const {
  if (name == widthAttr || name == heightAttr || name == alignAttr ||
      name == frameborderAttr)
    return true;
  return HTMLFrameElementBase::IsPresentationAttribute(name);
}
