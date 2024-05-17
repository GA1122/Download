 LayoutEmbeddedContent* HTMLFrameOwnerElement::GetLayoutEmbeddedContent() const {
  if (!GetLayoutObject() || !GetLayoutObject()->IsLayoutEmbeddedContent())
    return nullptr;
  return ToLayoutEmbeddedContent(GetLayoutObject());
}
