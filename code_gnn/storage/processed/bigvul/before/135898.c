DispatchEventResult TextTrackCue::DispatchEventInternal(Event* event) {
  if (!track() || track()->mode() == TextTrack::DisabledKeyword())
    return DispatchEventResult::kCanceledBeforeDispatch;

  return EventTarget::DispatchEventInternal(event);
}
