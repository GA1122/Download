static bool isImageOrAltText(LayoutBoxModelObject* box, Node* node) {
  if (box && box->isImage())
    return true;
  if (isHTMLImageElement(node))
    return true;
  if (isHTMLInputElement(node) &&
      toHTMLInputElement(node)->hasFallbackContent())
    return true;
  return false;
}
