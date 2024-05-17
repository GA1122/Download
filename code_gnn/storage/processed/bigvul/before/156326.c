void MediaRecorder::Trace(blink::Visitor* visitor) {
  visitor->Trace(stream_);
  visitor->Trace(dispatch_scheduled_event_runner_);
  visitor->Trace(scheduled_events_);
  EventTargetWithInlineData::Trace(visitor);
  PausableObject::Trace(visitor);
}
