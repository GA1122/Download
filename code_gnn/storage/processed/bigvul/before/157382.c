void HTMLMediaElement::ParserDidSetAttributes() {
  HTMLElement::ParserDidSetAttributes();

  if (FastHasAttribute(mutedAttr))
    muted_ = true;
}
