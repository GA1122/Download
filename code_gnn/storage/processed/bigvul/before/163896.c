DEFINE_TRACE(ImageCapture) {
  visitor->Trace(stream_track_);
  visitor->Trace(capabilities_);
  visitor->Trace(settings_);
  visitor->Trace(current_constraints_);
  visitor->Trace(photo_capabilities_);
  visitor->Trace(service_requests_);
  EventTargetWithInlineData::Trace(visitor);
  ContextLifecycleObserver::Trace(visitor);
}
