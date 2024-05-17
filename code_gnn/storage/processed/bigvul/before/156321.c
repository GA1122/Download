void MediaRecorder::OnError(const WebString& message) {
  DLOG(ERROR) << message.Ascii();
  StopRecording();
  ScheduleDispatchEvent(Event::Create(event_type_names::kError));
}
