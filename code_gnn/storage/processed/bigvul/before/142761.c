void HTMLMediaElement::CloneNonAttributePropertiesFrom(const Element& other,
                                                       CloneChildrenFlag flag) {
  HTMLElement::CloneNonAttributePropertiesFrom(other, flag);

  if (FastHasAttribute(kMutedAttr))
    muted_ = true;
}
