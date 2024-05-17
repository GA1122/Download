void MediaControlTimelineElement::setPosition(double currentTime) {
  setValue(String::number(currentTime));

  if (LayoutObject* layoutObject = this->layoutObject())
    layoutObject->setShouldDoFullPaintInvalidation();
}
