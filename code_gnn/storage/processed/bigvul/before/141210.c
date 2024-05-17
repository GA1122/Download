void Document::ScheduleUseShadowTreeUpdate(SVGUseElement& element) {
  use_elements_needing_update_.insert(&element);
  ScheduleLayoutTreeUpdateIfNeeded();
}
