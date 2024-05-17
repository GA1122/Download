void WebMediaPlayerImpl::ReportTimeFromForegroundToFirstFrame(
    base::TimeTicks foreground_time,
    base::TimeTicks new_frame_time) {
  base::TimeDelta time_to_first_frame = new_frame_time - foreground_time;
  if (HasAudio()) {
    UMA_HISTOGRAM_TIMES(
        "Media.Video.TimeFromForegroundToFirstFrame.DisableTrack",
        time_to_first_frame);
  } else {
    UMA_HISTOGRAM_TIMES("Media.Video.TimeFromForegroundToFirstFrame.Paused",
                        time_to_first_frame);
  }
}
