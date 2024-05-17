void HTMLMediaElement::didRecalcStyle() {
  if (layoutObject())
    layoutObject()->updateFromElement();
}
