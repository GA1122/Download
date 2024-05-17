bool HTMLInputElement::IsPresentationAttribute(
    const QualifiedName& name) const {
  if (name == vspaceAttr || name == hspaceAttr || name == alignAttr ||
      name == widthAttr || name == heightAttr ||
      (name == borderAttr && type() == InputTypeNames::image))
    return true;
  return TextControlElement::IsPresentationAttribute(name);
}
