void MediaRecorder::CreateBlobEvent(Blob* blob, double timecode) {
  ScheduleDispatchEvent(
      BlobEvent::Create(event_type_names::kDataavailable, blob, timecode));
}
