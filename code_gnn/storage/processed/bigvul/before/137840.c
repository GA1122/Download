MediaControlVolumeSliderElement* MediaControlVolumeSliderElement::create(
    MediaControls& mediaControls) {
  MediaControlVolumeSliderElement* slider =
      new MediaControlVolumeSliderElement(mediaControls);
  slider->ensureUserAgentShadowRoot();
  slider->setType(InputTypeNames::range);
  slider->setAttribute(stepAttr, "any");
  slider->setAttribute(maxAttr, "1");
  slider->setShadowPseudoId(
      AtomicString("-webkit-media-controls-volume-slider"));
  return slider;
 }
