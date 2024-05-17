MediaControlTimelineElement* MediaControlTimelineElement::create(
    MediaControls& mediaControls) {
  MediaControlTimelineElement* timeline =
      new MediaControlTimelineElement(mediaControls);
  timeline->ensureUserAgentShadowRoot();
  timeline->setType(InputTypeNames::range);
  timeline->setAttribute(stepAttr, "any");
  timeline->setShadowPseudoId(AtomicString("-webkit-media-controls-timeline"));
  return timeline;
}
