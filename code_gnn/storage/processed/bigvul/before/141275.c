void Document::UnscheduleUseShadowTreeUpdate(SVGUseElement& element) {
  use_elements_needing_update_.erase(&element);
}
