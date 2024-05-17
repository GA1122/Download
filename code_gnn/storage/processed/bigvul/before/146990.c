WebLocalFrameImpl* WebLocalFrameImpl::FromFrameOwnerElement(Element* element) {
  if (!element->IsFrameOwnerElement())
    return nullptr;
  return FromFrame(
      ToLocalFrame(ToHTMLFrameOwnerElement(element)->ContentFrame()));
}
