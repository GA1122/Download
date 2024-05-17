void HTMLMediaElement::DidRecalcStyle(StyleRecalcChange) {
  if (GetLayoutObject())
    GetLayoutObject()->UpdateFromElement();
}
