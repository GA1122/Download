void HTMLFormControlElement::didRecalcStyle(StyleRecalcChange) {
  if (LayoutObject* layoutObject = this->layoutObject())
    layoutObject->updateFromElement();
}
