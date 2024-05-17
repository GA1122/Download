void HTMLMediaElement::InvokeResourceSelectionAlgorithm() {
  BLINK_MEDIA_LOG << "invokeResourceSelectionAlgorithm(" << (void*)this << ")";
  SetNetworkState(kNetworkNoSource);


  played_time_ranges_ = TimeRanges::Create();

  last_seek_time_ = 0;
  duration_ = std::numeric_limits<double>::quiet_NaN();

  SetShouldDelayLoadEvent(true);
  if (GetMediaControls())
    GetMediaControls()->Reset();

  ScheduleNextSourceChild();
}
