void MediaControlsProgressView::UpdateProgress(
    const media_session::MediaPosition& media_position) {
  if (media_position.playback_rate() == 0 && update_progress_timer_.IsRunning())
    update_progress_timer_.Stop();

  base::TimeDelta current_position = media_position.GetPosition();
  base::TimeDelta duration = media_position.duration();

  double progress = current_position.InSecondsF() / duration.InSecondsF();
  SetBarProgress(progress);

  base::DurationFormatWidth time_format =
      duration >= base::TimeDelta::FromDays(1) ? base::DURATION_WIDTH_NARROW
                                               : base::DURATION_WIDTH_NUMERIC;

  base::string16 elapsed_time;
  bool elapsed_time_received = base::TimeDurationFormatWithSeconds(
      current_position, time_format, &elapsed_time);

  base::string16 total_time;
  bool total_time_received =
      base::TimeDurationFormatWithSeconds(duration, time_format, &total_time);

  if (elapsed_time_received && total_time_received) {
    if (duration < base::TimeDelta::FromHours(1)) {
      base::ReplaceFirstSubstringAfterOffset(
          &elapsed_time, 0, base::ASCIIToUTF16("0:"), base::ASCIIToUTF16(""));
      base::ReplaceFirstSubstringAfterOffset(
          &total_time, 0, base::ASCIIToUTF16("0:"), base::ASCIIToUTF16(""));
    }

    SetProgressTime(elapsed_time);
    SetDuration(total_time);
  }

  if (media_position.playback_rate() != 0) {
    base::TimeDelta update_frequency = base::TimeDelta::FromSecondsD(
        std::abs(1 / media_position.playback_rate()));
    update_progress_timer_.Start(
        FROM_HERE, update_frequency,
        base::Bind(&MediaControlsProgressView::UpdateProgress,
                   base::Unretained(this), media_position));
  }
 }
