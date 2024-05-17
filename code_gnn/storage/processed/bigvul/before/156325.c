void MediaRecorder::StopRecording() {
  DCHECK(state_ != State::kInactive);
  state_ = State::kInactive;

  recorder_handler_->Stop();

  WriteData(nullptr  , 0  , true  ,
            WTF::CurrentTimeMS());
  ScheduleDispatchEvent(Event::Create(event_type_names::kStop));
}
