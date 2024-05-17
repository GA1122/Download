void MediaControlTimelineElement::setDuration(double duration) {
  setFloatingPointAttribute(maxAttr, std::isfinite(duration) ? duration : 0);

  if (LayoutObject* layoutObject = this->layoutObject())
    layoutObject->setShouldDoFullPaintInvalidation();
}
