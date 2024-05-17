void HTMLMediaElement::ParserDidSetAttributes() {
  HTMLElement::ParserDidSetAttributes();

  if (FastHasAttribute(kMutedAttr))
    muted_ = true;
}
