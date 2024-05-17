void HTMLMediaElement::CloneNonAttributePropertiesFrom(const Element& other,
                                                       CloneChildrenFlag flag) {
  HTMLElement::CloneNonAttributePropertiesFrom(other, flag);

  if (FastHasAttribute(mutedAttr))
    muted_ = true;
}
