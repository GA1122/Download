void MediaControlVolumeSliderElement::setVolume(double volume) {
  if (value().toDouble() == volume)
    return;

  setValue(String::number(volume));
  if (LayoutObject* layoutObject = this->layoutObject())
    layoutObject->setShouldDoFullPaintInvalidation();
}
