bool TabLifecycleUnitSource::TabLifecycleUnit::IsMediaTabImpl(
    DecisionDetails* decision_details) const {

  bool is_media_tab = false;

  if (recently_audible_time_ == base::TimeTicks::Max() ||
      (!recently_audible_time_.is_null() &&
       NowTicks() - recently_audible_time_ < kTabAudioProtectionTime)) {
    is_media_tab = true;
    if (decision_details) {
      decision_details->AddReason(
          DecisionFailureReason::LIVE_STATE_PLAYING_AUDIO);
    }
  }

  scoped_refptr<MediaStreamCaptureIndicator> media_indicator =
      MediaCaptureDevicesDispatcher::GetInstance()
          ->GetMediaStreamCaptureIndicator();

  if (media_indicator->IsCapturingUserMedia(GetWebContents())) {
    is_media_tab = true;
    if (decision_details)
      decision_details->AddReason(DecisionFailureReason::LIVE_STATE_CAPTURING);
  }

  if (media_indicator->IsBeingMirrored(GetWebContents())) {
    is_media_tab = true;
    if (decision_details)
      decision_details->AddReason(DecisionFailureReason::LIVE_STATE_MIRRORING);
  }

  return is_media_tab;
}
